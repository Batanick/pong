#include "Label.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "commonMath.h"

void Label::onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId) {
  fontColorId = glGetUniformLocation(shaderId, "oColor");
  textureParamId = glGetUniformLocation(shaderId, "texture");
  textureId = font->getTextureInfo().textureId;
}

void Label::onBeforeRender(const RenderContext &context) {
  glUniform3f(fontColorId, color.r, color.g, color.b);
}

void Label::setText(const RenderContext &context, std::string text) {
  if (text == str) {
    return;
  }

  const TextureInfo info = font->getTextureInfo();
  glm::mat3 const uvProj = convertToViewMatrix(1 / (float)info.width, 1 / (float)info.height, 0, 0);

  std::vector<glm::vec2> vertices;
  std::vector<glm::vec2> uvs;

  int currentX = x;
  for (unsigned int i = 0; i < str.length(); i++) {
    int const chr = str[i];
    Glyph const gl = font->getGlyph(chr);

    glm::vec2 p1(glm::vec3(currentX, y, 1) * context.guiView);
    glm::vec2 p2(glm::vec3(currentX + gl.width, y, 1) * context.guiView);
    glm::vec2 p3(glm::vec3(currentX + gl.width, y + gl.height, 1) * context.guiView);
    glm::vec2 p4(glm::vec3(currentX, y + gl.height, 1) * context.guiView);

    glm::vec2 uv1(glm::vec3(gl.x, gl.y + gl.height, 1) * uvProj);
    glm::vec2 uv2(glm::vec3(gl.x + gl.width, gl.y + gl.height, 1) * uvProj);
    glm::vec2 uv3(glm::vec3(gl.x + gl.width, gl.y, 1) * uvProj);
    glm::vec2 uv4(glm::vec3(gl.x, gl.y, 1) * uvProj);

    vertices.push_back(p1);
    uvs.push_back(uv1);
    vertices.push_back(p2);
    uvs.push_back(uv2);
    vertices.push_back(p4);
    uvs.push_back(uv4);
    vertices.push_back(p2);
    uvs.push_back(uv2);
    vertices.push_back(p3);
    uvs.push_back(uv3);
    vertices.push_back(p4);
    uvs.push_back(uv4);

    currentX += gl.width + font->getDistance();
  }
  
  setVertices(uvs, vertices);

  str = text;
}