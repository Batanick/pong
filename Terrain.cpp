#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

void Terrain::init() {
	if ( (widthTiles <= 0) || (heightTiles <= 0)) {
		return;
	}

	std::vector<glm::vec2> verticies;
	int y = 0;
	for (float y = 0; y < (tileSize * heightTiles + 1); y += tileSize){
		for (float x = 0; x < (tileSize * (widthTiles + 1)); x += tileSize) {
			verticies.push_back( glm::vec2(x, y) );
		}
	}
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, verticies.size() * sizeof(glm::vec2), &verticies[0], GL_STATIC_DRAW );

	std::vector<unsigned short> indices;
	int indidiesNeeded = heightTiles * (2 * (widthTiles + 1) - 1) + 1;  

	indices.push_back(0);

	int rowEndCounter = 2 * widthTiles + 1;
	bool forward = true;
	bool right = true;
	int current = 0;
	for (int i = 1; i < indidiesNeeded ; i++){
		if (forward) {
			current += widthTiles + 1;
		} else {
			current -= widthTiles + 1;
			current += right ? 1 : -1;
		}

		indices.push_back(current);
		forward = !forward; 

		if ( (--rowEndCounter) == 0 ) {
			forward = true;
			right = !right;
			rowEndCounter = 2 * widthTiles + 1;
		}
	}

	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned short), &indices[0], GL_STATIC_DRAW );
	indicesSize = indices.size();
}

void Terrain::render( const RenderContext &context ) {
	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements( GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_SHORT, (void *) 0);

	glDisableVertexAttribArray(0);
}
