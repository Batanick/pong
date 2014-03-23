#include "RenderableMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

void RenderableMesh::init() {
    textureId = GL_INVALID_VALUE;

    std::vector<const glm::vec3> vertices;
	std::vector<const unsigned int> indices;
    std::vector<const glm::vec2> uvs;

    initMesh( vertices, indices, uvs );
    indicesSize = indices.size();

    glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) , &vertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), &indices[0], GL_STATIC_DRAW );
	indicesSize = indices.size();

    initTexture( textureId );
}

void RenderableMesh::render( const RenderContext &context ) {
    glUniformMatrix4fv( context.meshMVPId, 1, GL_FALSE, &context.pv[0][0] );

  	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    if ( textureId != GL_INVALID_VALUE  ) {
        glActiveTexture( GL_TEXTURE0 );
    	glBindTexture( GL_TEXTURE_2D, textureId );
	    glUniform1i( context.meshTextureUniformId, 0 );

        glEnableVertexAttribArray(1);
    	glBindBuffer( GL_ARRAY_BUFFER, uvsBuffer );
	    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    }
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void *) 0); 

	glDisableVertexAttribArray(0);

    if ( textureId != GL_INVALID_VALUE ) {
	    glDisableVertexAttribArray(1);
    }
}

void RenderableMesh::shutdown() {
    glDeleteBuffers ( 1, &vertexBuffer );
	glDeleteBuffers ( 1, &indexBuffer );

    if ( textureId != GL_INVALID_VALUE ) {
	    glDeleteBuffers ( 1, &uvsBuffer );
    }
}