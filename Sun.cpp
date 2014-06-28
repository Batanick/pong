#include "Sun.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "commonMath.h"

void Sun::initVertices(std::vector<const VertexData> &vertices, std::vector<const unsigned short> &indices) {
  vertices.push_back(VertexData(glm::vec3(-MESH_SIZE, MESH_SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(MESH_SIZE, MESH_SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(MESH_SIZE, -MESH_SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));
  vertices.push_back(VertexData(glm::vec3(-MESH_SIZE, -MESH_SIZE, 0.0f), glm::vec2(0.0f, 0.0f)));

  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(1);
}

void Sun::initTexture(GLuint &textureId) {
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

glm::mat4 Sun::getModelTransform(const RenderContext &context) {
  const glm::vec3 dirToSun = glm::normalize(-context.lightDir);
  const glm::vec3 translation = dirToSun / dirToSun.y * MESH_HEIGHT;
  const glm::mat4 transMat = glm::translate(glm::mat4(), translation + context.cameraPos);

  const glm::quat rotationQuat = getRotation(-zAxis, context.lightDir);
  return transMat * glm::toMat4(rotationQuat);
}