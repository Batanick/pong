// =============================================================================================
// ============================= MAGIC NUMBERS KINGDOM =========================================
// =============================================================================================

#ifndef H_RENDER_TERRAIN_CONSTANTS
#define H_RENDER_TERRAIN_CONSTANTS

static const float MAX_HEIGHT = 128;

static const float PATCH_SIZE_METERS = 128;

static const int PATCHES_COUNT_SQRT = 64;
static const int PATCHES_COUNT = PATCHES_COUNT_SQRT * PATCHES_COUNT_SQRT;

static const int TILES_IN_PATCH_SQRT = 128;
static const int TILES_IN_PATCH = TILES_IN_PATCH_SQRT * TILES_IN_PATCH_SQRT;
static const int VERTICES_IN_PATH = (TILES_IN_PATCH_SQRT + 1) * (TILES_IN_PATCH_SQRT + 1);

static const float TILE_SIZE = PATCH_SIZE_METERS / TILES_IN_PATCH_SQRT;

static const float TERRAIN_SIZE = PATCH_SIZE_METERS * PATCHES_COUNT_SQRT;
static const float TERRAIN_SIZE_HALF = TERRAIN_SIZE / 2;
static const float TERRAIN_OFFSET = -TERRAIN_SIZE_HALF;

static const int LOD_LEVELS_COUNT = 4;
static const int LOD_STEP = 1;

struct color {
  color() {
    r = 0;
    g = 0;
    b = 0;
    maxDisloc = 0;
  };

  color(unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char maxDisloc = 0) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->maxDisloc = maxDisloc;
  }

  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char maxDisloc;
};

#include <vector>
std::vector< std::pair<float, color>> buildTerrainColors();

#endif