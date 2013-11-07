#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "assetLoader.h"
#include "logging.h"

#include "RenderCommon.h"
#include "TextureManager.h"

void Mesh::init( TextureManager &textureManager ) {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;
	std::vector<glm::vec2> uvs;

	if ( !loadFromFile( modelPath, vertices, indices, uvs ) ) {
		return;
	}
	
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) , &vertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned short ), &indices[0], GL_STATIC_DRAW );
	indicesSize = indices.size();

	loadTexture( textureManager, uvs );
}

void Mesh::loadTexture( TextureManager &textureManager, std::vector<glm::vec2> &uvs ) {
	if ( texturePath.empty() ) {
		return;
	}

	if ( uvs.size() == 0 ){
		return;	
	}

	glGenBuffers( 1, &uvsBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, uvsBuffer );
	glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	textureId = textureManager.loadTexture( texturePath );
}

void Mesh::render( const RenderContext &context ) {
	static int frameNum = 0;
	const glm::mat4 mvp = context.pv* glm::rotate(glm::mat4(1), ((float) (frameNum++)), glm::vec3(0,1,0)); 
	glUniformMatrix4fv( context.meshMVPId, 1, GL_FALSE, &mvp[0][0] );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textureId );
	glUniform1i( context.meshTextureUniformId, 0 );
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer( GL_ARRAY_BUFFER, uvsBuffer );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_SHORT, (void *) 0); 

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Mesh::shutdown() {
	glDeleteBuffers ( 1, &vertexBuffer );
	glDeleteBuffers ( 1, &indexBuffer );
	glDeleteBuffers ( 1, &uvsBuffer );
}

