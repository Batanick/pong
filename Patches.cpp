#include "Patches.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "logging.h"
#include "noise.h"

int countLevelOfDetail(const int &x, const int &y);
float getHeight(float x, float y);
void generateVertices(const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod);

void Patches::init() {
  patches.resize(PATCHES_COUNT);

  std::vector<GLuint> vBuffers(PATCHES_COUNT);
  glGenBuffers(PATCHES_COUNT, &vBuffers[0]);
  const long dataSize = VERTICES_IN_PATH * sizeof(glm::vec3);

  for (int i = 0; i < PATCHES_COUNT; i++) {
    pMutex lock = std::shared_ptr<std::mutex>(new std::mutex());
    locks.push_back(lock);

    Patch &patch = patches[i];
    patch.id = vBuffers[i];
    patch.lod = -1;
    patch.x = 0;
    patch.y = 0;
    patch.vertices.clear();
    patch.normals.clear();

    glBindBuffer(GL_ARRAY_BUFFER, patch.id);

    glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW);
  }

  running = true;
  backgndThread = std::make_shared<std::thread>(std::thread(&Patches::refreshThread, this));
}

void Patches::refresh() {
  const glm::vec3 cameraPos = position;

  const int cameraX = static_cast<int>(cameraPos.x / PATCH_SIZE_METERS);
  const int cameraY = static_cast<int>(cameraPos.z / PATCH_SIZE_METERS);

  for (int i = 0; i < PATCHES_COUNT; i++) {
    static const int offset = PATCHES_COUNT_SQRT / 2;
    const int offsetX = i % PATCHES_COUNT_SQRT - offset;
    const int offsetY = i / PATCHES_COUNT_SQRT - offset;
    const int suggestedLod = countLevelOfDetail(offsetX, offsetY);

    const int patchX = offsetX + cameraX;
    const int patchY = offsetY + cameraY;

    const Patch &patch = patches[i]; // no need of sincronization here, since we are the only thread that writing this data
    const bool needReinit = (patch.lod < 0) || (patch.lod > suggestedLod) || (patch.x != patchX) || (patch.y != patchY);

    if (needReinit) {
      reinitPatch(i, patchX, patchY, suggestedLod);
    }

    if (!running)
      return;
  }
}

void Patches::reinitPatch(const int &index, const int &x, const int &y, const int &lod) {
  const glm::vec2 offset((x - 0.5f) * PATCH_SIZE_METERS, (y - 0.5f) * PATCH_SIZE_METERS);
  std::vector<glm::vec3> vertices;
  generateVertices(offset, vertices, lod);

  {
    PatchHolder holder = acquire(index);
    Patch &patch = holder.patch;
    patch.lod = lod;
    patch.x = x;
    patch.y = y;
    patch.vertices = vertices;
  }
}

PatchHolder Patches::acquire(const int &index) {
  Patch& patch = patches[index];
  std::mutex &lock = *locks[index];

  PatchHolder holder = PatchHolder(patch, lock);
  return holder;
}

void Patches::updatePos(const glm::vec3 &updatePos) {
  position = updatePos;
}

void Patches::refreshThread() {
  while (running) {
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

void Patches::shutdown() {
  running = false;
  backgndThread->join();

  for (const auto &patch : patches) {
    glDeleteBuffers(1, &patch.id);
  }
}

int countLevelOfDetail(const int &x, const int &y) {
  const int lodLevel = glm::max(static_cast<int>(glm::abs(x + 0.5f)), static_cast<int>(glm::abs(y + 0.5f))) / LOD_STEP;
  const int lod = glm::min(glm::max(0, lodLevel), LOD_LEVELS_COUNT - 1);
  return lod;
}

void generateVertices(const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod) {
  const double start = glfwGetTime();

  const int factor = glm::min(TILES_IN_PATCH_SQRT, 1 << lod * LOD_REDUCTION);
  const float tileSize = TILE_SIZE * factor;
  const int tilesCount = TILES_IN_PATCH_SQRT / factor;

  for (int y = 0; y < tilesCount + 1; y++) {
    for (int x = 0; x < tilesCount + 1; x++) {
      const float xCoord = x * tileSize + offset.x;
      const float yCoord = y * tileSize + offset.y;
      const float zCoord = getHeight(xCoord, yCoord); // * MAX_HEIGHT;
      vertices.push_back(glm::vec3(xCoord, zCoord, yCoord));
    }
  }

  if (lod == 0) {
    LOG("Reiniting lod[%d]: %dms", lod, static_cast<int>((glfwGetTime() - start) * 1000));
  }
}

float getHeight(float x, float y) {
  float result = glm::max(noise(x / 64, y / 64) + 0.8f, 0.0f) / 2.0f;
  return result * MAX_HEIGHT;
}