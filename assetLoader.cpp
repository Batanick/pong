#include "assetLoader.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include <assimp/postprocess.h>  

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging.h"

bool loadTriangle( std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices ) {
	vertices.push_back(glm::vec3 (-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3 (1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3 (0.0f,  1.0f, 0.0f));

	for (unsigned int i = 0; i < 3; i++) {
		indices.push_back( (unsigned short) i );
	}

	return true;
}

bool loadFromFile( const char *fileName, std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices, std::vector<glm::vec2> &uvs ) {
	LOG ("Loading model %s", fileName);

	Assimp::Importer imrporter;
	const aiScene *scene =  imrporter.ReadFile( fileName, aiProcess_JoinIdenticalVertices ); // pointer handled by importer
	VERIFY( scene, imrporter.GetErrorString(), return false );

	//TODO: fix more then one mesh case
	for ( unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++ ) {
		const aiMesh* currentMesh = scene->mMeshes[meshIndex];

		vertices.reserve( currentMesh->mNumVertices );
		for ( unsigned int vertexIndex = 0; vertexIndex < currentMesh->mNumVertices; vertexIndex++ ) {
			const aiVector3D aiVec3 = currentMesh->mVertices[vertexIndex];
			vertices.push_back( glm::vec3( aiVec3.x, aiVec3.y, aiVec3.z ) );
		}
	
		indices.reserve( currentMesh->mNumFaces * 3 );
		for ( unsigned int faceIndex = 0; faceIndex < currentMesh->mNumFaces; faceIndex++ ) {
			const aiFace face = currentMesh->mFaces[faceIndex];

			for ( unsigned int i = 0; i < face.mNumIndices; i++ ) {
				indices.push_back( (unsigned short) face.mIndices[i] );
			}
		}	

		if ( currentMesh->GetNumUVChannels() > 0 ) {
			uvs.reserve ( currentMesh->mNumVertices );
			for ( unsigned int vertexIndex = 0; vertexIndex < currentMesh->mNumVertices; vertexIndex++ ) {
				const aiVector3D aiVec3 = currentMesh->mTextureCoords[0][vertexIndex];
				uvs.push_back( glm::vec2(aiVec3.x, aiVec3.y) );
			}
		}

	}

	LOG ( "Meshes: [%d], vertices: [%d], indices: [%d], uvs: [%d]", scene->mNumMeshes, vertices.size(), indices.size(), uvs.size() );

	return true;
}

/* 
* Texture loading originally taken from http://www.opengl-tutorial.org/
*/

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

bool loadDDS( const std::string fileName ) {
	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	errno_t err = fopen_s( &fp, fileName.c_str(), "rb" ); 
	if (err != 0) 
		return false; 
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return false; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return false; 
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 
	} 

	free(buffer); 

	return true;
}