#include "renderUtils.h"

#include "glm.hpp"
#include <gtc\matrix_transform.hpp>

void generateIndexTable(const int rows, const int cols, std::vector<unsigned int> &indices) {
  int indidiesNeeded = rows * (2 * (cols + 1) - 1) + 1;

  indices.push_back(0);

  int rowEndCounter = 2 * cols + 1;
  bool forward = true;
  bool right = true;
  int current = 0;
  for (int i = 1; i < indidiesNeeded; i++){
    if (forward) {
      current += cols + 1;
    }
    else {
      current -= cols + 1;
      current += right ? 1 : -1;
    }

    indices.push_back(current);
    forward = !forward;

    if ((--rowEndCounter) == 0) {
      forward = true;
      right = !right;
      rowEndCounter = 2 * cols + 1;

      //repeating last element to reset triangle direction
      indices.push_back(indices.back());
      indices.push_back(indices.back()); 
    }
  }
}

void generateRowIndices(const int offset, const int cols, std::vector<unsigned int> &indices) {
  for (int i = offset; i < offset + cols; i++) {
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + cols + 1);

    indices.push_back(i + 1);
    indices.push_back(i + cols + 1);
    indices.push_back(i + cols + 2);
  }
}

glm::mat4 getViewMatrix(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up) {
  return glm::lookAt(pos, pos + dir, up);
}

void setUpCamera(RenderContext &context, const glm::vec3 &pos, const glm::vec3 &up, const glm::vec3 &direction) {
  context.cameraPos = pos;
  context.cameraUp = up;
  context.cameraDir = direction;

  context.view = getViewMatrix(pos, direction, up);
  context.projection = glm::perspective(45.0f, ((float)context.windowWidth) / context.windowHeight, 1.0f, 5000.0f);
  context.pv = context.projection * context.view;
}