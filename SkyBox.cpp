#include "SkyBox.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <vector>

static std::vector< glm::vec3> vertices = {
  glm::vec3(-SIZE, -SIZE, -SIZE),
  glm::vec3(SIZE, -SIZE, -SIZE),
  glm::vec3(-SIZE, -SIZE, SIZE),
  glm::vec3(SIZE, -SIZE, SIZE),
  glm::vec3(-SIZE, SIZE, -SIZE),
  glm::vec3(SIZE, SIZE, -SIZE),
  glm::vec3(-SIZE, SIZE, SIZE),
  glm::vec3(SIZE, SIZE, SIZE),
};

static std::vector<unsigned short> indices = {
  0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
};

void SkyBox::init(const GLuint shaderId) {
  mvpId = glGetUniformLocation(shaderId, "mvp");
  cameraPosId = glGetUniformLocation(shaderId, "cameraPos");

  glGenBuffers(1, &verticesId);
  glBindBuffer(GL_ARRAY_BUFFER, verticesId);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &indicesId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned short), &indices[0], GL_STATIC_DRAW);
}

void SkyBox::render(const RenderContext &context) {
  glUniformMatrix4fv(mvpId, 1, GL_FALSE, &context.pv[0][0]);
  glUniform3fv(cameraPosId, 1, &context.cameraPos[0]);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, verticesId);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
  glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_SHORT, (void *)0);

  glDisableVertexAttribArray(0);
}

void SkyBox::shutdown() {
  glDeleteBuffers(1, &verticesId);
  glDeleteBuffers(1, &indicesId);
}