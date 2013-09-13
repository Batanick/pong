#include "assetLoader.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include <assimp/postprocess.h>  

#include "logging.h"

bool loadTriangle( std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices ) {
	vertices.push_back(glm::vec3 (-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3 (1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3 (0.0f,  1.0f, 0.0f));
	vertices.push_back(glm::vec3 (1.0f,  1.0f, 1.0f));

	for (unsigned int i = 0; i < 3; i++) {
		indices.push_back( (unsigned short) i );
	}
	for (unsigned int i = 1; i <= 3; i++) {
		indices.push_back( (unsigned short) i );
	}

	return true;
}

bool loadFromFile( const char *fileName, std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices ) {
	LOG ("Loading model %s", fileName);

	Assimp::Importer imrporter;
	const aiScene *scene =  imrporter.ReadFile( fileName, aiProcess_JoinIdenticalVertices ); // pointer handled by importer
	VERIFY( scene, imrporter.GetErrorString(), return false );

	int indexCounter = 0;
	int vertexCounter = 0;
	int facesCounter = 0;

	for ( unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++ ) {
		const aiMesh* currentMesh = scene->mMeshes[meshIndex];

		for ( unsigned int vertexIndex = 0; vertexIndex < currentMesh->mNumVertices; vertexIndex++ ) {
			const aiVector3D aiVec3 = currentMesh->mVertices[vertexIndex];
			vertices.push_back( glm::vec3( aiVec3.x, aiVec3.y, aiVec3.z ) );
		}
		vertexCounter += currentMesh->mNumVertices;

		for ( unsigned int faceIndex = 0; faceIndex < currentMesh->mNumFaces; faceIndex++ ) {
			const aiFace face = currentMesh->mFaces[faceIndex];

			for ( unsigned int i = 0; i < face.mNumIndices; i++ ) {
				indices.push_back( (unsigned short) face.mIndices[i] );
			}
			indexCounter += face.mNumIndices;
		}	
		facesCounter += currentMesh->mNumFaces;
	}

	LOG ( "Meshes: [%d], Faces: [%d], vertices: [%d], indices: [%d]", scene->mNumMeshes, facesCounter, vertexCounter, indexCounter );

	return true;
}

bool loadTexture( const char *fileName ) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile( fileName, 0 ); // handled by importer obj
	VERIFY( scene, importer.GetErrorString(), return false );


	return true;
}