#include <vector>
#include <glm.hpp>

struct Texel {
	unsigned char r, g, b, a;
};

struct TextureData {
	int width, height;
	unsigned int format;

	std::vector<Texel> data;
};

bool loadTriangle( std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices );
bool loadFromFile( const char *fileName, std::vector<glm::vec3> &vertices,  std::vector<unsigned short> &indices );
bool loadTexture( const char *fileName, TextureData &data );