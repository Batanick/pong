#include "RenderCommon.h"
#include <vector>
#include <memory>

class HeightMap;

class Terrain final : public Renderable {

public:
    Terrain(int tiles):
		vertexBuffer(-1),
		indexBuffer(-1),
		indicesSize(-1), 
        tileSize( 0.1f ),
        tiles (tiles) {
	}

	void init();
	virtual void render( const RenderContext &context );
	virtual void shutdown();
    
    glm::vec3 getRandomPos();

private :
	GLuint vertexBuffer;
	GLuint indexBuffer;
	int indicesSize;

    float const tileSize;
    int const tiles;

    std::shared_ptr<HeightMap> heightMap;

    float getHeight( float x, float y );

	void generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices );
};