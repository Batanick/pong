#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "noise.h"
#include "renderUtils.h"

void Terrain::init( const GLuint shaderId ) {
	VERIFY( isPower2( tiles ), "Width value is not a power of 2", return );

	std::vector<glm::vec3> verticies;
	generateVertices( tiles, tileSize, verticies );
	
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, verticies.size() * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW );

	std::vector<unsigned int> indices;
    generateIndexTable( tiles, tiles, indices );
	
	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );
	indicesSize = indices.size();

    mvpId = glGetUniformLocation( shaderId, "mvp" );
}

void Terrain::render( const RenderContext &context ) {
    glUniformMatrix4fv( mvpId, 1, GL_FALSE, &context.pv[0][0] );

	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements( GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
}

void Terrain::generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices ) {
    const float offset = res * tileSize / 2;

	for ( int y = 0; y < res + 1; y++) {
		for (int x = 0; x < res + 1; x++) {
            vertices.push_back ( glm::vec3(x * tileSize - offset, getHeight( x * tileSize - offset, y * tileSize - offset), y * tileSize - offset) );
		}
	}
}

float Terrain::getHeight( float x, float y ) {
    return noise( x ,y );
}

glm::vec3 Terrain::getRandomPos() {
    const float x = tiles * tileSize * getRandomFloat() - offset;
    const float z = tiles * tileSize * getRandomFloat() - offset;
    
    return glm::vec3( x, getHeight(x, z), z );
}


void Terrain::shutdown() {
    glDeleteBuffers ( 1, &vertexBuffer );
    glDeleteBuffers ( 1, &indexBuffer );
}