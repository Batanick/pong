#include "RenderableMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

void RenderableMesh::init(const GLuint shaderId) {
  std::vector<const TexVertexData> vertices;
  std::vector<const unsigned int> indices;

  initMesh(vertices, indices);
  indicesSize = indices.size();

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexVertexData), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  indicesSize = indices.size();

  initTexture(textureId);

  projectionId = glGetUniformLocation(shaderId, "projection");
  viewId = glGetUniformLocation(shaderId, "view");
  lightDirId = glGetUniformLocation(shaderId, "lightDir");
  textureParamId = glGetUniformLocation(shaderId, "texture");
}

void RenderableMesh::render(const RenderContext &context) {
  glUniformMatrix4fv(projectionId, 1, GL_FALSE, &context.projection[0][0]);
  glUniformMatrix4fv(viewId, 1, GL_FALSE, &context.view[0][0]);
  glUniform3fv(lightDirId, 1, &context.lightDir[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glUniform1i(textureParamId, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  static const int coordsOffset = offsetof(TexVertexData, position);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *)coordsOffset);

  static const int normalOffset = offsetof(TexVertexData, normal);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *)normalOffset);

  static const int uvOffset = offsetof(TexVertexData, uv);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *)uvOffset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *)0);

  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}

void RenderableMesh::shutdown() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &indexBuffer);
}

