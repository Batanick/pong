#include <vector>

#include "renderCommon.h"

void generateIndexTable(const int rows, const int cols, std::vector<unsigned int> &indices);

void generateRowIndices(const int offset, const int cols, std::vector<unsigned int> &indices);

void generateTriangleIndices(const int offset, const int rows, const int cols, std::vector<const unsigned int> &indices);

glm::mat3 convertToViewMatrix(float scaleX, float scaleY, int translateX, int translateY);

void setUpCamera(RenderContext &context, const glm::vec3 &pos, const glm::vec3 &up, const glm::vec3 &direction);