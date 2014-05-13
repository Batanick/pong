#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "noise.h"
#include "renderUtils.h"

int countLevelOfDetail( const int &x, const int &y );

void Terrain::init( const GLuint shaderId ) {
    indexBuffers.resize(LOD_LEVELS_COUNT);
    std::vector<GLuint> iBuffers( LOD_LEVELS_COUNT );
    glGenBuffers( LOD_LEVELS_COUNT, &iBuffers[0] );

    int size = TILES_IN_PATCH_SQRT;
    for (int i = 0; i < LOD_LEVELS_COUNT; i++) {
        std::vector<unsigned int> indices;

        generateIndexTable( size, size, indices );
        LOG("Lod %d: [%d] elements", i, size);
        size = size >> LOD_REDUCTION;

        indexBuffers[i].id = iBuffers[i];
        indexBuffers[i].length = indices.size();
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].id );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].length * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );
    }
        
    patches.resize( PATCHES_COUNT );
    indexToLod.resize( PATCHES_COUNT );

    std::vector<GLuint> vBuffers( PATCHES_COUNT );
    glGenBuffers( PATCHES_COUNT, &vBuffers[0] );
    for ( int i = 0; i < PATCHES_COUNT; i++ ) {
        Patch &patch = patches[i];
        patch.id = vBuffers[i];

        const int row = i / PATCHES_COUNT_SQRT;
        const int column = i % PATCHES_COUNT_SQRT;

        const int lod = countLevelOfDetail( column, row );
        patch.lod = lod;
        indexToLod[i] = lod;

        glBindBuffer( GL_ARRAY_BUFFER, patch.id );
        
        const long dataSize = VERTICES_IN_PATH * sizeof(glm::vec3);
        glBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW );

        reinitPatch( patch, column, row, LOD_LEVELS_COUNT - 1 );
    }
    
    mvpId = glGetUniformLocation( shaderId, "mvp" );
}

bool Terrain::reinitPatch( Patch &patch, const int x, const int y, int lod ) {
    patch.lod = lod;
    std::vector<glm::vec3> vertices;
    const glm::vec2 offset( position.x + x * PATCH_SIZE_METERS + TERRAIN_OFFSET, position.z + y * PATCH_SIZE_METERS + TERRAIN_OFFSET );
    generateVertices( offset, vertices, patch.lod );

    glBindBuffer( GL_ARRAY_BUFFER, patch.id );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0] );

    return true;
}

void Terrain::render( const RenderContext &context ) {
    glUniformMatrix4fv( mvpId, 1, GL_FALSE, &context.pv[0][0] );
    
    glEnableVertexAttribArray(0);
    for (int i = 0; i < PATCHES_COUNT; i++) {
        const Patch &patch = patches[i];
		if (patch.lod == -1) {
			continue;
		}

        glBindBuffer( GL_ARRAY_BUFFER, patch.id );
	    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

        const IndexBuffer &indexBuffer = indexBuffers[patch.lod];
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
        glDrawElements( GL_TRIANGLE_STRIP, indexBuffer.length, GL_UNSIGNED_INT, (void *) 0);

    }
    glDisableVertexAttribArray(0);
}

bool Terrain::refresh( const RenderContext &context ) {
    const glm::vec3 cameraPos = context.cameraPos;
    const float dx = cameraPos.x - position.x;
    const float dz = cameraPos.z - position.z;

    if ( (abs(dx) > PATCH_SIZE_METERS * 0.66) || (abs(dz) > PATCH_SIZE_METERS * 0.66) ) {
		rebuildTerrain(dx, dz);
    }

	int reinitCounter = 0;
	for (int i = 0; i < PATCHES_COUNT; i++) {
		// using global index here to avoid sittuation where some patches refresh more often then others
		refreshPos = (refreshPos + 1) % PATCHES_COUNT;
		Patch &patch = patches[refreshPos];
		if (patch.lod != indexToLod[refreshPos]) {
			reinitPatch(patch, refreshPos % PATCHES_COUNT_SQRT, refreshPos / PATCHES_COUNT_SQRT, indexToLod[refreshPos]);
			reinitCounter++;

			if (reinitCounter >= LOD_REINIT_LIMIT)
				return false;
		}
	}
	
	return reinitCounter <= 0;
}

void Terrain::rebuildTerrain( const float &dx, const float &dz) {
    const int dxPatches = static_cast<int>( glm::round(dx / PATCH_SIZE_METERS) );
    const float dxActual = dxPatches * PATCH_SIZE_METERS;
    const int dzPatches = static_cast<int>( glm::round(dz / PATCH_SIZE_METERS) );
    const float dzActual = dzPatches * PATCH_SIZE_METERS;
    
    position = position + glm::vec3(dxActual, 0, dzActual);
       
    std::vector<Patch> patchesCopy(patches.begin(), patches.end());
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

		if (needReinit) {
			patches[i].lod = -1; // don't render until refresh_patch
		}
    }
}

void Terrain::generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod ) {
    const int factor = glm::min(TILES_IN_PATCH_SQRT, 1 << lod * LOD_REDUCTION);
    const float tileSize = TILE_SIZE * factor;
    const int tilesCount = TILES_IN_PATCH_SQRT / factor;

    for ( int y = 0; y < tilesCount + 1; y++) {
        for (int x = 0; x < tilesCount + 1; x++) {
            const float xCoord = x * tileSize + offset.x;
            const float yCoord = y * tileSize + offset.y;
            vertices.push_back ( glm::vec3(xCoord, getHeight( xCoord, yCoord), yCoord) );
		}
	}
}

float Terrain::getHeight( float x, float y ) {
	return MAX_HEIGHT * noise(x / 128, y / 128);
}

glm::vec3 Terrain::getRandomPos() {
    const float x = 0.0;//tiles * tileSize * getRandomFloat() - offset;
    const float z = 0.0;//tiles * tileSize * getRandomFloat() - offset;
    
    return glm::vec3( x, getHeight(x, z), z );
}


void Terrain::shutdown() {
    for (const auto buffer : indexBuffers ) {
        glDeleteBuffers ( 1, &buffer.id );
    }
    
    for (const auto patch : patches ) {
        glDeleteBuffers ( 1, &patch.id );
    }
}

int countLevelOfDetail( const int &x, const int &y ) {
    static const float indexOffset = PATCHES_COUNT_SQRT / 2 - 0.5;
    const float floatLod =  glm::max( glm::abs(static_cast<float>(x) - indexOffset), glm::abs(static_cast<float>(y) - indexOffset) ) / LOD_STEP;
    const int lod = glm::min( glm::max(0, static_cast<int>(floatLod)), LOD_LEVELS_COUNT - 1 );
    return lod;
}