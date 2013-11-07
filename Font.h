#ifndef H_FONT
#define H_FONT

#include "RenderCommon.h"

class Font final { 

public:
	Font( GLuint textureId ): 
		textureId( textureId ) {
	}

	void init();
	void shutdown();
	GLuint getTextureId();

private:
	const GLuint textureId;

};

#endif