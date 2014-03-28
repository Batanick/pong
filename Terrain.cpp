#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "diamondGen.h"
#include "renderUtils.h"

void Terrain::init( ) {
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
}

void Terrain::render( const RenderContext &context ) {
    glUniformMatrix4fv( context.terrainMVPId, 1, GL_FALSE, &context.pv[0][0] );

    glUniform2f( context.terrainMinMaxId, heightMap->getMinHeight(), heightMap->getMaxHeight() );

	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements( GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
}

void Terrain::generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices ) {
    heightMap = std::shared_ptr<HeightMap> (HeightMap::create(0.0f, 0.0f, 0.0f, 0.0f, res + 1));

    const float offset = res * tileSize / 2;

	for ( int y = 0; y < res + 1; y++) {
		for (int x = 0; x < res + 1; x++) {
            vertices.push_back ( glm::vec3(x * tileSize - offset, heightMap->getHeight(x, y), y * tileSize - offset) );
		}
	}
}

float Terrain::getHeight( float x, float y ) {
    const int tileX = (int) (x / tileSize);
    const int tileY = (int) (y / tileSize);

    if ( tileX >= tiles || tileY >= tiles)
        return - 1;

    return (heightMap->getHeight(tileX, tileY) + heightMap->getHeight(tileX + 1, tileY + 1)) / 2;
}

glm::vec3 Terrain::getRandomPos() {
    const float x = tiles * tileSize * getRandomFloat();
    const float z = tiles * tileSize * getRandomFloat();

    const float offset = tiles * tileSize / 2;
    
    return glm::vec3( x - offset, getHeight( x, z), z - offset );
}


void Terrain::shutdown() {
    glDeleteBuffers ( 1, &vertexBuffer );
    glDeleteBuffers ( 1, &indexBuffer );
}