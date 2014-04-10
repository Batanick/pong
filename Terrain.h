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

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
    
    glm::vec3 getRandomPos();

private :
	GLuint vertexBuffer;
	GLuint indexBuffer;

    GLuint mvpId;
    GLuint minMaxHeight;

	int indicesSize;

    float const tileSize;
    int const tiles;

    std::shared_ptr<HeightMap> heightMap;

    float getHeight( float x, float y );

	void generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices );
};