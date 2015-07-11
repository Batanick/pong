#include "TexturedFrame.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "commonMath.h"

void TexturedFrame::onInit(const GLuint shaderId, GLuint &textureId, GLuint &textureParamId) {
    colorId = glGetUniformLocation(shaderId, "oColor");
    textureParamId = glGetUniformLocation(shaderId, "texture");
    textureId = this->textureId;

    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> uvs;

    glm::vec2 p1(x, y);
    glm::vec2 p2(x + width, y);
    glm::vec2 p3(x + width, y + height);
    glm::vec2 p4(x, y + height);

    glm::vec2 uv1(0, 1);
    glm::vec2 uv2(1, 1);
    glm::vec2 uv3(1, 0);
    glm::vec2 uv4(0, 0);

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

    setVertices(uvs, vertices);
}

void TexturedFrame::onBeforeRender(const RenderContext &context) {
    glUniform3f(colorId, 1, 1, 1);
}

