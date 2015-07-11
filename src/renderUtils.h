#include <vector>

#include "RenderCommon.h"

void generateIndexTable(const int rows, const int cols, std::vector<unsigned int> &indices);

void generateRowIndices(const int offset, const int cols, std::vector<unsigned int> &indices);

template<typename T>
void generateQuadIndices(std::vector<T> &indices, int offset, const bool bothSides) {
    indices.push_back(0 + offset);
    indices.push_back(1 + offset);
    indices.push_back(2 + offset);

    indices.push_back(0 + offset);
    indices.push_back(2 + offset);
    indices.push_back(1 + offset);

    if (bothSides) {
        indices.push_back(0 + offset);
        indices.push_back(2 + offset);
        indices.push_back(3 + offset);

        indices.push_back(0 + offset);
        indices.push_back(3 + offset);
        indices.push_back(2 + offset);
    }
}

void generateTriangleIndices(const int offset, const int rows, const int cols,
                             std::vector<unsigned int> &indices);

glm::mat3 convertToViewMatrix(float scaleX, float scaleY, int translateX, int translateY);

void setUpCamera(RenderContext &context, const glm::vec3 &pos, const glm::vec3 &up, const glm::vec3 &direction);