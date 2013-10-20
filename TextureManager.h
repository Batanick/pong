#include <map>
#include <memory>
#include <string>

#include "RenderCommon.h"

class TextureManager {

public:
	GLuint loadTexture( std::string fileName );
	GLuint getTextureId( std::string fileName );

	void shutdown();

private:
	std::map<std::string, GLuint> textures;

};
