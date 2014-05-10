#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "noise.h"
#include "renderUtils.h"

//#define DEBUG_TERRAIN_REINIT
#define STATIC_TERRAIN

void Terrain::init( const GLuint shaderId ) {
    std::vector<unsigned int> indices;
    generateIndexTable( TILES_IN_PATCH_SQRT, TILES_IN_PATCH_SQRT, indices );
    indicesSize = indices.size();
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );
    
    patches.resize( PATCHES_COUNT );
    glGenBuffers( PATCHES_COUNT, &patches[0] );
    for ( int i = 0; i < PATCHES_COUNT; i++ ) {
        const GLuint bufferId = patches[i];
        reinitPatch( bufferId, i % PATCHES_COUNT_SQRT, i / PATCHES_COUNT_SQRT );
    }
    
    mvpId = glGetUniformLocation( shaderId, "mvp" );
}

void Terrain::reinitPatch( const GLuint &patch, const int x, const int y ) {
    std::vector<glm::vec3> vertices;

    const glm::vec2 offset( position.x + x * PATCH_SIZE_METERS + TERRAIN_OFFSET, position.z + y * PATCH_SIZE_METERS + TERRAIN_OFFSET );
    generateVertices( offset, vertices );

    glBindBuffer( GL_ARRAY_BUFFER, patch );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW );
}

void Terrain::render( const RenderContext &context ) {
    refresh(context);

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

void Terrain::refresh( const RenderContext &context ) {
#ifdef STATIC_TERRAIN
    if (true) return ;
#endif

    const glm::vec3 cameraPos = context.cameraPos;
    const float dx = cameraPos.x - position.x;
    const float dz = cameraPos.z - position.z;

    if ( (abs(dx) < PATCH_SIZE_METERS) && (abs(dz) < PATCH_SIZE_METERS) ) {
        return;
    }

    const int dxPatches = static_cast<int>( glm::round(dx / PATCH_SIZE_METERS) );
    const float dxActual = dxPatches * PATCH_SIZE_METERS;
    const int dzPatches = static_cast<int>( glm::round(dz / PATCH_SIZE_METERS) );
    const float dzActual = dzPatches * PATCH_SIZE_METERS;
    
    position = position + glm::vec3(dxActual, 0, dzActual);
   
#ifdef DEBUG_TERRAIN_REINIT
    LOG("Partial rebuild dx:%d dz:%d", dxPatches, dzPatches);
#endif
    
    std::vector<GLuint> patchesCopy(patches.begin(), patches.end());
    std::vector<GLuint> freeId;
    std::vector<int> reinitIndices;

    for (int i = 0; i < PATCHES_COUNT; i++) {
        const int column = i % PATCHES_COUNT_SQRT;
        const int row = i / PATCHES_COUNT_SQRT; 
        bool needReinit = false;

        int oldColumn = column + dxPatches;
        if (oldColumn < 0 || oldColumn >= PATCHES_COUNT_SQRT) {
            oldColumn = (oldColumn + PATCHES_COUNT) % PATCHES_COUNT_SQRT;
            needReinit = true;
        }
        
        int oldRow = row + dzPatches;
        if (oldRow < 0 || oldRow >= PATCHES_COUNT_SQRT) {
            oldRow = (oldRow + PATCHES_COUNT) % PATCHES_COUNT_SQRT;
            needReinit = true;
        }
        
        const int oldIndex = oldColumn + oldRow * PATCHES_COUNT_SQRT;
        patches[i] = patchesCopy[oldIndex];

#ifdef DEBUG_TERRAIN_REINIT
        LOG("%d<-%d:%s", i, oldIndex, needReinit ? "reinit" : "");
#endif
      
        if (needReinit) {
            reinitPatch(patches[i], column, row);
        }
    }

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
    return MAX_HEIGHT * noise( x / 128 , y  / 128);
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