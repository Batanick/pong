#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assetLoader.h"

void Mesh::init() {
	if ( !loadFromFile( "../models/suzanne.obj", vertices, indices ) ) {
		return;
	}

	if ( !loadTexture("../models/testTexture.DDS") ){
		return;
	}

	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) , &vertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned short ), &indices[0], GL_STATIC_DRAW );
}

void Mesh::render( const RenderContext context ) {
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer (
		0,                
		3 , 
		GL_FLOAT, 
		GL_FALSE, 
		0,        
		(void*)0   
		);

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void *) 0); 

	glDisableVertexAttribArray(0);
}

glm::mat4 Mesh::getModelTrans() {
	return model;
}

void Mesh::shutdown() {
	glDeleteBuffers ( 1, &vertexBuffer );
	glDeleteBuffers ( 1, &indexBuffer );
}

