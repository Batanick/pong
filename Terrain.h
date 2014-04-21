#include "RenderCommon.h"
#include <vector>
#include <memory>

static const float PATCH_SIZE_METERS = 16;

static const int PATCHES_COUNT_SQRT = 2;
static const int PATCHES_COUNT = PATCHES_COUNT_SQRT * PATCHES_COUNT_SQRT;

static const int TILES_IN_PATCH_SQRT = 16;
static const int TILES_IN_PATCH = TILES_IN_PATCH_SQRT * TILES_IN_PATCH_SQRT;

static const float TILE_SIZE = PATCH_SIZE_METERS / TILES_IN_PATCH_SQRT;
static const float TERRAIN_OFFSET = - PATCH_SIZE_METERS * PATCHES_COUNT_SQRT / 2;

class Terrain final : public Renderable {

public:
    Terrain() {
        position = glm::vec3();
	}

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
    
    glm::vec3 getRandomPos();
    float getHeight( float x, float y );

private :
    std::vector<GLuint> patches;

    glm::vec3 position;
    
    GLuint mvpId;

    GLuint indexBuffer;
    int indicesSize;

	void generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices );

    glm::vec2 countOffset( const int x, const int y );
    void reinitPatch( const GLuint &patch, const int x, const int y );
    
};