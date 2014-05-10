#include "RenderCommon.h"
#include <vector>
#include <memory>

static const float MAX_HEIGHT = 32;

static const float PATCH_SIZE_METERS = 128;

static const int PATCHES_COUNT_SQRT = 32;
static const int PATCHES_COUNT = PATCHES_COUNT_SQRT * PATCHES_COUNT_SQRT;

static const int TILES_IN_PATCH_SQRT = 256;
static const int TILES_IN_PATCH = TILES_IN_PATCH_SQRT * TILES_IN_PATCH_SQRT;

static const float TILE_SIZE = PATCH_SIZE_METERS / TILES_IN_PATCH_SQRT;

static const float TERRAIN_SIZE = PATCH_SIZE_METERS * PATCHES_COUNT_SQRT;
static const float TERRAIN_SIZE_HALF = TERRAIN_SIZE / 2; 
static const float TERRAIN_OFFSET = - TERRAIN_SIZE_HALF;

static const int LOD_LEVELS_COUNT = 8; 
static const int LOD_STEP = 2;

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
    struct IndexBuffer {
        GLuint id;
        int length;
    };

    struct Patch {
        GLuint id;
        int lod;
    };

    std::vector<Patch> patches;
    std::vector<IndexBuffer> indexBuffers;

    glm::vec3 position;
    
    GLuint mvpId;

	void generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod );

    void refresh( const RenderContext &context );
    void reinitPatch( Patch &patch, const int x, const int y );
};