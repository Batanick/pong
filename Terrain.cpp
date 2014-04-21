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
    std::vector<unsigned int> indices;
    generateIndexTable( TILES_IN_PATCH_SQRT, TILES_IN_PATCH_SQRT, indices );
    indicesSize = indices.size();
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );
    
    patches.resize( PATCHES_COUNT );
    glGenBuffers( PATCHES_COUNT, &patches[0] );
    for ( int x = 0; x < PATCHES_COUNT_SQRT; x++ ) 
        for ( int y = 0; y < PATCHES_COUNT_SQRT; y++ ) {
            const GLuint bufferId = patches[x + y * PATCHES_COUNT_SQRT];
            reinitPatch( bufferId, x, y );
        }
    
    mvpId = glGetUniformLocation( shaderId, "mvp" );
}

void Terrain::reinitPatch( const GLuint &patch, const int x, const int y ) {
    std::vector<glm::vec3> vertices;

    const glm::vec2 offset( position.x + x * PATCH_SIZE_METERS + TERRAIN_OFFSET, position.y + y * PATCH_SIZE_METERS + TERRAIN_OFFSET );
    generateVertices( offset, vertices );

    glBindBuffer( GL_ARRAY_BUFFER, patch );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW );
}

void Terrain::render( const RenderContext &context ) {
    glUniformMatrix4fv( mvpId, 1, GL_FALSE, &context.pv[0][0] );
    
    glEnableVertexAttribArray(0);
    for (int i = 0; i < PATCHES_COUNT; i++) {
        glBindBuffer( GL_ARRAY_BUFFER, patches[i] );
	    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

	    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    	glDrawElements( GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *) 0);

    }
    glDisableVertexAttribArray(0);
}

void Terrain::generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices ) {
	for ( int y = 0; y < TILES_IN_PATCH_SQRT + 1; y++) {
		for (int x = 0; x < TILES_IN_PATCH_SQRT + 1; x++) {
            const float xCoord = x * TILE_SIZE + offset.x;
            const float yCoord = y * TILE_SIZE + offset.y;
            vertices.push_back ( glm::vec3(xCoord, getHeight( xCoord, yCoord), yCoord) );
		}
	}
}

float Terrain::getHeight( float x, float y ) {
    return 0.0f;
    //return noise( x / 2 ,y / 2 );
}

glm::vec3 Terrain::getRandomPos() {
    const float x = 0.0;//tiles * tileSize * getRandomFloat() - offset;
    const float z = 0.0;//tiles * tileSize * getRandomFloat() - offset;
    
    return glm::vec3( x, getHeight(x, z), z );
}


void Terrain::shutdown() {
    glDeleteBuffers ( 1, &indexBuffer );
    glDeleteBuffers ( PATCHES_COUNT, &patches[0] );
}