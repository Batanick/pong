#include "GuiElement.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

void GuiElement::init(const GLuint shaderId) {
  glGenBuffers(1, &vertexBuffer);
  glGenBuffers(1, &uvBuffer);

  onInit(shaderId, textureId, textureParamId);
}

void GuiElement::render(const RenderContext &context) {
  if (!loaded) {
    reinit();
    loaded = true;
  }

  GLint currentMode[2]; //front and back value
  glGetIntegerv(GL_POLYGON_MODE, &currentMode[0]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  onBeforeRender(context);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glUniform1i(textureParamId, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glPolygonMode(GL_FRONT_AND_BACK, currentMode[0] == GL_LINE ? GL_LINE : GL_FILL);
}

void GuiElement::reinit() {
  if (vertices.empty()) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_DYNAMIC_DRAW);

  loaded = true;
}

void GuiElement::setVertices(const std::vector<glm::vec2> &uvs, const std::vector<glm::vec2> &vertices) {
  this->uvs = uvs;
  this->vertices = vertices;

  loaded = false;
}

void GuiElement::shutdown() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &uvBuffer);
}

