#include <map>
#include <memory>
#include <string>

#include "RenderCommon.h"

class TextureManager final {

public:
	GLuint loadTexture( std::string fileName );
	GLuint getTextureId( std::string fileName );

	void shutdown();

private:
	std::map<std::string, GLuint> textures;

};
