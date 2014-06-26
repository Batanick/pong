#include "TexturedMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "logging.h"

#include <vector>
#include <limits>

void TexturedMesh::init(const GLuint shaderId) {
  mvpId = glGetUniformLocation(shaderId, "mvp");
  textureParamId = glGetUniformLocation(shaderId, "texture");

  std::vector<const VertexData> vertices;
  std::vector<const unsigned short> indices;
  initVertices(vertices, indices);
  indicesCount = indices.size();
  if (vertices.size() > USHRT_MAX){
    LOG("Too many vertices count: [%d]", vertices.size());
    return;
  }

  initTexture(textureId);

  glGenBuffers(1, &verticesId);
  glBindBuffer(GL_ARRAY_BUFFER, verticesId);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_DYNAMIC_DRAW);

  glGenBuffers(1, &indicesId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned short), &indices[0], GL_DYNAMIC_DRAW);
}

void TexturedMesh::render(const RenderContext &context) {
  const glm::mat4 transform = context.pv * getModelTransform(context);
  glUniformMatrix4fv(mvpId, 1, GL_FALSE, &transform[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glUniform1i(textureParamId, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, verticesId);

  static const int posOffset = offsetof(VertexData, position);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)posOffset);

  static const int uvOffset = offsetof(VertexData, uv);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)uvOffset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
  glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_SHORT, (void *)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void TexturedMesh::shutdown() {
  glDeleteBuffers(1, &verticesId);
  glDeleteTextures(1, &textureId);
}

static const float SIZE = 64;

void TexturedMesh::initVertices(std::vector<const VertexData> &vertices, std::vector<const unsigned short> &indices) {
  vertices.push_back(VertexData(glm::vec3(-SIZE, SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(SIZE, SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(SIZE, -SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(-SIZE, -SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));

  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(1);
}

void TexturedMesh::initTexture(GLuint &textureId) {
  const int textureLength = 64;
  const int textureSize = textureLength * textureLength * 3 * sizeof(unsigned char);
  unsigned char *textureData = (unsigned char *)malloc(textureSize);

  for (int x = 0; x < textureLength; x++) {
    for (int y = 0; y < textureLength; y++) {

      const int i = x + y * textureLength;
      textureData[i * 3] = 255;			  // R 
      textureData[i * 3 + 1] = 255;		// G 
      textureData[i * 3 + 2] = 0;		  // B 
    }
  }

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, textureLength, textureLength);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureLength, textureLength, GL_RGB, GL_UNSIGNED_BYTE, textureData);

  free(textureData);
}

glm::mat4 TexturedMesh::getModelTransform(const RenderContext &context){
  return glm::mat4();
}