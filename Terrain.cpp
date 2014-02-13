#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "diamondGen.h"

void Terrain::init( const float tileSize, const int tiles ) {
	VERIFY( isPower2( tiles ), "Width value is not a power of 2", return );

	std::vector<glm::vec3> verticies;
	generateVertices( tiles, tileSize, verticies );
	
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, verticies.size() * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW );

	std::vector<unsigned int> indices;
	generateIndicies( tiles, indices );
	
	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );
	indicesSize = indices.size();
}

void Terrain::render( const RenderContext &context ) {
    glUniformMatrix4fv( context.terrainMVPId, 1, GL_FALSE, &context.pv[0][0] );

	glUniform2f( context.terrainMinMaxId, minHeight, maxHeight );

	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements( GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
}

void Terrain::generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices ) {
	const HeightMap heightMap = HeightMap::create(0.0f, 0.0f, 0.0f, 0.0f, res + 1);

	for ( int y = 0; y < res + 1; y++) {
		for (int x = 0; x < res + 1; x++) {
			vertices.push_back ( glm::vec3(x * tileSize, heightMap.getHeight(x, y), y * tileSize) );
		}
	}

	minHeight = heightMap.getMinHeight();
	maxHeight = heightMap.getMaxHeight();
}

void Terrain::generateIndicies( const int res, std::vector<unsigned int> &indices ) {
	int indidiesNeeded = res * (2 * (res + 1) - 1) + 1;  

	indices.push_back(0);

	int rowEndCounter = 2 * res + 1;
	bool forward = true;
	bool right = true;
	int current = 0;
	for (int i = 1; i < indidiesNeeded ; i++){
		if (forward) {
			current += res + 1;
		} else {
			current -= res + 1;
			current += right ? 1 : -1;
		}

		indices.push_back(current);
		forward = !forward; 

		if ( (--rowEndCounter) == 0 ) {
			forward = true;
			right = !right;
			rowEndCounter = 2 * res + 1;
		}
	}

}

void Terrain::shutdown() {
    glDeleteBuffers ( 1, &vertexBuffer );
    glDeleteBuffers ( 1, &indexBuffer );
}