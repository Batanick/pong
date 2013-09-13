#include <vector>
#include <glm.hpp>

typedef unsigned int GLuint;

bool loadTriangle( std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices );
bool loadFromFile( const char *fileName, std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices, std::vector<glm::vec2> &uvs );
bool loadDDS(const std::string imagepath );