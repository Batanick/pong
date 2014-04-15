#include "RenderCommon.h"
#include <vector>
#include <memory>

class Terrain final : public Renderable {

public:
    Terrain(int tiles):
		vertexBuffer(-1),
		indexBuffer(-1),
		indicesSize(-1), 
        tileSize( 0.1f ),
        tiles (tiles), 
        offset (tiles * tileSize / 2){
	}

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
    
    glm::vec3 getRandomPos();
    float getHeight( float x, float y );

private :
	GLuint vertexBuffer;
	GLuint indexBuffer;

    GLuint mvpId;

    int indicesSize;

    float const tileSize;
    float const offset;
    int const tiles;

	void generateVertices( const int res, const float tileSize, std::vector<glm::vec3> &vertices );
};