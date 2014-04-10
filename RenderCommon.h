#ifndef H_RENDER_COMMON
#define H_RENDER_COMMON
#include "glm.hpp"

static const float RENDER_PI = 3.14159265358979323846264338327950288f;
static const float RENDER_FOV = 45.0f;

typedef unsigned int GLuint;

struct RenderContext {
	double timeDelta;

    int windowHeight;
    int windowWidth;

	glm::mat4 pv;
};

struct Glyph {
    Glyph() {
    }
    Glyph(int key, int x, int y, int width, int height):
        key(key),
        x(x), y(y),
        width(width), 
        height(height){
    }

    int key;
    int x,y;
    int width, height;
};

struct TextureInfo {
    TextureInfo(){
    }

    TextureInfo(GLuint textureId, unsigned int width, unsigned int height):
        textureId(textureId),
        height(height),
        width(width){
    }

    GLuint textureId;
    unsigned int height;
    unsigned int width;
};

class Renderable {
public:
    virtual void render( const RenderContext &context ) = 0;
    virtual void shutdown() = 0;
    virtual void init( const GLuint shaderId ) = 0;
};

#endif