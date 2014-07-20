#include "Water.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

void Water::init(const GLuint shaderId) {
  timeId = glGetUniformLocation(shaderId, "time");
  mainColorId = glGetUniformLocation(shaderId, "mainColor");
  projectionId = glGetUniformLocation(shaderId, "projection");
  viewId = glGetUniformLocation(shaderId, "view");
  cameraPosId = glGetUniformLocation(shaderId, "cameraPos");
  reflectionTexId = glGetUniformLocation(shaderId, "reflectionTex");
  lightDirId = glGetUniformLocation(shaderId, "lightDirection");

  std::vector<glm::vec3> vertices;

  vertices.push_back(glm::vec3(-WATER_SIZE, 0, WATER_SIZE));
  vertices.push_back(glm::vec3(WATER_SIZE, 0, WATER_SIZE));
  vertices.push_back(glm::vec3(-WATER_SIZE, 0, -WATER_SIZE));

  vertices.push_back(glm::vec3(-WATER_SIZE, 0, -WATER_SIZE));
  vertices.push_back(glm::vec3(WATER_SIZE, 0, WATER_SIZE));
  vertices.push_back(glm::vec3(WATER_SIZE, 0, -WATER_SIZE));
  
  glGenBuffers(1, &verticesId);
  glBindBuffer(GL_ARRAY_BUFFER, verticesId);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}

void Water::render(const RenderContext &context) {
  const float time = (float)context.time;
  glUniform1f(timeId, time);
  glUniform3fv(mainColorId, 1, &COLOR[0]);
  glUniform3fv(lightDirId, 1, &context.lightDir[0]);
  glUniform3fv(cameraPosId, 1, &context.cameraPos[0]);
  glUniformMatrix4fv(viewId, 1, GL_FALSE, &context.view[0][0]);
  glUniformMatrix4fv(projectionId, 1, GL_FALSE, &context.projection[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, context.reflectionTexId);
  glUniform1i(reflectionTexId, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, verticesId);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(0);
}

void Water::shutdown() {
  glDeleteBuffers(1, &verticesId);
}

ShaderType Water::getType() {
  return ShaderType::WATER_SHADER;
}
