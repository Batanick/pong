#include "RenderCommon.h"
#include <vector>
#include <memory>

static const float MAX_HEIGHT = 32;

static const float PATCH_SIZE_METERS = 128;

static const int PATCHES_COUNT_SQRT = 32;
static const int PATCHES_COUNT = PATCHES_COUNT_SQRT * PATCHES_COUNT_SQRT;

static const int TILES_IN_PATCH_SQRT = 256;
static const int TILES_IN_PATCH = TILES_IN_PATCH_SQRT * TILES_IN_PATCH_SQRT;
static const int VERTICES_IN_PATH = (TILES_IN_PATCH_SQRT + 1) * (TILES_IN_PATCH_SQRT + 1);

static const float TILE_SIZE = PATCH_SIZE_METERS / TILES_IN_PATCH_SQRT;

static const float TERRAIN_SIZE = PATCH_SIZE_METERS * PATCHES_COUNT_SQRT;
static const float TERRAIN_SIZE_HALF = TERRAIN_SIZE / 2; 
static const float TERRAIN_OFFSET = - TERRAIN_SIZE_HALF;

static const int LOD_LEVELS_COUNT = 4; 
static const int LOD_STEP = 2;
static const int LOD_REDUCTION = 2;

static const int PATCH_REINIT_LIMIT = 2;
static const int PATCH_MEMORY_UPDATE_LIMIT = 1;

class Terrain final : public Renderable {

public:
    Terrain() {
        position = glm::vec3();
		refreshPos = 0;
	}

    virtual void init( const GLuint shaderId ) override;
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
	virtual bool refresh( const RenderContext &context ) override;
    
    glm::vec3 getRandomPos();
    float getHeight( float x, float y );

private :
    struct IndexBuffer {
        GLuint id;
        int length;
    };

    struct Patch {
		Patch() {
			x = -1;
			y = -1;
		}

        GLuint id;
        int lod;
		std::vector<glm::vec3> vertices;
		int x, y;
    };

    std::vector<Patch> patches;
    std::vector<IndexBuffer> indexBuffers;
    std::vector<int> indexToLod;

    glm::vec3 position;
    
    GLuint mvpId;

	unsigned int refreshPos;

	void generateVertices( const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod );
	
    void rebuildTerrain( const float &dx, const float &dz);
    void reinitPatch( Patch &patch, const int x, const int y, int lod );
};