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

ShaderType TexturedMesh::getType() {
  return ShaderType::TEX_MESH_SHADER;
}