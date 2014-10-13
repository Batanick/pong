#include "RenderableMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

void RenderableMesh::init(const GLuint shaderId) {
  std::vector<const VertexData> vertices;
  std::vector<const unsigned int> indices;

  initMesh(vertices, indices);
  indicesSize = indices.size();

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  indicesSize = indices.size();

  projectionId = glGetUniformLocation(shaderId, "projection");
  viewId = glGetUniformLocation(shaderId, "view");
  lightDirId = glGetUniformLocation(shaderId, "lightDir");
  colorId = glGetUniformLocation(shaderId, "vColor");
}

void RenderableMesh::render(const RenderContext &context) {
  glUniformMatrix4fv(projectionId, 1, GL_FALSE, &context.projection[0][0]);
  glUniformMatrix4fv(viewId, 1, GL_FALSE, &context.view[0][0]);
  glUniform3fv(lightDirId, 1, &context.lightDir[0]);
  glUniform3fv(colorId, 1, &color[0]);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  static const int coordsOffset = offsetof(VertexData, position);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)coordsOffset);

  static const int normalOffset = offsetof(VertexData, normal);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)normalOffset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, (void *)0);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}

void RenderableMesh::shutdown() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &indexBuffer);
}

void RenderableMesh::setColor(float r, float g, float b) {
  color = glm::vec3(r, g, b);
}

