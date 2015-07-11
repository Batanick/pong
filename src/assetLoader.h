#include <string>
#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

bool loadTriangle(std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices);

bool loadFromFile(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices,
                  std::vector<glm::vec2> &uvs);

bool loadDDS(const std::string fileName, unsigned int &width, unsigned int &height);