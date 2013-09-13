#include <vector>
#include <glm.hpp>

bool loadTriangle( std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices );
bool loadFromFile( const char *fileName, std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices );
bool loadTexture( const char *fileName );