#include "Terrain.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "logging.h"
#include "commonMath.h"
#include "renderUtils.h"

void Terrain::init(const GLuint shaderId) {
  mvpId = glGetUniformLocation(shaderId, "mvp");
  textureParamId = glGetUniformLocation(shaderId, "terrainTexture");
  heightId = glGetUniformLocation(shaderId, "heightScale");

  initIndices(shaderId);
  initVertices(shaderId);
  initTexture(shaderId);
}

void Terrain::initIndices(const GLuint &shaderId) {
  indexBuffers.resize(LOD_LEVELS_COUNT);
  std::vector<GLuint> iBuffers(LOD_LEVELS_COUNT);
  glGenBuffers(LOD_LEVELS_COUNT, &iBuffers[0]);

  int size = TILES_IN_PATCH_SQRT;
  for (int i = 0; i < LOD_LEVELS_COUNT; i++) {
    std::vector<unsigned int> indices;

    generateIndexTable(size, size, indices);
    LOG("Lod %d: [%d] elements", i, size);
    size = size >> LOD_REDUCTION;

    indexBuffers[i].id = iBuffers[i];
    indexBuffers[i].length = indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].length * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  }
}

void Terrain::initVertices(const GLuint &shaderId) {
  patches->init();
}

void Terrain::initTexture(const GLuint &shaderId) {
  const int textureLength = 64;
  const int textureSize = textureLength * 3 * sizeof(unsigned char);
  unsigned char *textureData = (unsigned char *)malloc(textureSize);

  const std::vector< std::pair<float, color>> colors = buildTerrainColors();

  unsigned int n = 0;
  for (int i = 0; i < textureLength; i++) {
    const float u = ((float)i) / textureLength;

    if ((colors[n].first < u) && (n < (colors.size() - 1))) {
      n++;
    }

    const color clr = colors[n].second;
    textureData[i * 3] = clr.r;			  // R 
    textureData[i * 3 + 1] = clr.g;		// G 
    textureData[i * 3 + 2] = clr.b;		// B 
  }

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_1D, textureId);
  glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, textureLength);
  glTexSubImage1D(GL_TEXTURE_1D, 0, 0, textureLength, GL_RGB, GL_UNSIGNED_BYTE, textureData);

  free(textureData);
}

void Terrain::render(const RenderContext &context) {
  patches->updatePos(context.cameraPos);

  glUniformMatrix4fv(mvpId, 1, GL_FALSE, &context.pv[0][0]);
  glUniform1f(heightId, MAX_HEIGHT);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D, textureId);
  glUniform1i(textureParamId, 0);

  glEnableVertexAttribArray(0);
  int updateCounter = 0;

  for (int i = 0; i < PATCHES_COUNT; i++) {
    PatchHolder holder = patches->acquire(i);
    Patch &patch = holder.patch;
    if (!patch.needToDraw()) {
      continue;
    }
    refreshPatch(patch);

    glBindBuffer(GL_ARRAY_BUFFER, patch.id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    const IndexBuffer &indexBuffer = indexBuffers[patch.lod];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
    glDrawElements(GL_TRIANGLE_STRIP, indexBuffer.length, GL_UNSIGNED_INT, (void *)0);
  }
  glDisableVertexAttribArray(0);
}

void Terrain::refreshPatch(Patch &patch) {
  if (!patch.needReinit()) {
    return;
  }

  const double start = glfwGetTime();

  glBindBuffer(GL_ARRAY_BUFFER, patch.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, patch.vertices.size() * sizeof(glm::vec3), &patch.vertices[0]);
  glFinish();

  patch.normals.clear();
  patch.vertices.clear();

  if (patch.lod == 0) {
    LOG("Refreshing lod[%d]: %dms", patch.lod, static_cast<int>((glfwGetTime() - start) * 1000));
  }
}

void Terrain::shutdown() {
  for (const auto buffer : indexBuffers) {
    glDeleteBuffers(1, &buffer.id);
  }

  patches->shutdown();
  glDeleteTextures(1, &textureId);
}


