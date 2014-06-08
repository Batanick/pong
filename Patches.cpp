#include "Patches.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "logging.h"
#include "noise.h"

float getHeight(float x, float y);
void generateVertices(const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod);
int normalizeOffset(const int &value);

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

    const int patchX = i % PATCHES_COUNT_SQRT - PATCHES_COUNT_SQRT / 2;
    const int patchY = i / PATCHES_COUNT_SQRT - PATCHES_COUNT_SQRT / 2;

    reinitPatch(i, patchX, patchY, countLevelOfDetail(patchX, patchY));
  }

  running = true;
  backgndThread = std::make_shared<std::thread>(std::thread(&Patches::refreshThread, this));
}

void Patches::refresh() {
  glm::vec3 cameraPos = cameraPosition;

  const glm::vec2 currentPos = glm::vec2(offsetX * PATCH_SIZE_METERS, offsetY * PATCH_SIZE_METERS);
  const glm::vec2 posDelta = glm::vec2(cameraPos.x - currentPos.x, cameraPos.z - currentPos.y);

  static const float posThreshold = PATCH_SIZE_METERS;
  if ((glm::abs(posDelta.x) < posThreshold) && (glm::abs(posDelta.y) < posThreshold)){
    //return;
  }

  offsetX = static_cast<int>(glm::round(cameraPos.x / PATCH_SIZE_METERS));
  offsetY = static_cast<int>(glm::round(cameraPos.z / PATCH_SIZE_METERS));

  int reinited = 0;
  for (int i = 0; i < PATCHES_COUNT; i++) {
    const int patchX = i % PATCHES_COUNT_SQRT + offsetX - PATCHES_COUNT_SQRT / 2;
    const int patchXNorm = normalizeOffset(patchX);

    const int patchY = i / PATCHES_COUNT_SQRT + offsetY - PATCHES_COUNT_SQRT / 2;
    const int patchYNorm = normalizeOffset(patchY);

    const int actualIndex = patchXNorm + PATCHES_COUNT_SQRT * patchYNorm;
    const Patch &patch = patches[actualIndex];

    int suggestedLod = 0;
    bool needReinit = (patch.x != patchX) || (patch.y != patchY);
    if (needReinit) {
      suggestedLod = LOD_LEVELS_COUNT - 1;
    }
    else {
      suggestedLod = glm::max(patch.lod - 1, countLevelOfDetail(patchX - offsetX, patchY - offsetY));
      needReinit |= suggestedLod != patch.lod; 
    }

    if (needReinit) {
      reinitPatch(actualIndex, patchX, patchY, suggestedLod);
      reinited++;
    }

    if (!running)
      return;
  }

  LOG("Reinited: %d", reinited);
}

void Patches::reinitPatch(const int &index, const int &x, const int &y, const int &lod) {
  const glm::vec2 offset(x * PATCH_SIZE_METERS, y * PATCH_SIZE_METERS);
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
  cameraPosition = updatePos;
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

int Patches::countLevelOfDetail(const int &x, const int &y) {
  const int xLod = static_cast<int>(glm::abs(x + 0.5f));
  const int yLod = static_cast<int>(glm::abs(y + 0.5f));
  return glm::min(LOD_LEVELS_COUNT - 1, glm::max(0, glm::max(xLod, yLod)));
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
      const float zCoord = getHeight(xCoord, yCoord);
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

int normalizeOffset(const int &value) {
  int result = value;

  if (result >= PATCHES_COUNT_SQRT) {
    while (result >= PATCHES_COUNT_SQRT) {
      result -= PATCHES_COUNT_SQRT;
    }
    return result;
  }
  else {
    while (result < 0) {
      result += PATCHES_COUNT_SQRT;
    }

    return result;
  }
}
