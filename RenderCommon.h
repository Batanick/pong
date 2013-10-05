#ifndef H_RENDER_COMMON
#define H_RENDER_COMMON
#include "glm.hpp"

static const float RENDER_PI = 3.14159265358979323846264338327950288f;
static const float RENDER_FOV = 45.0f;

typedef unsigned int GLuint;

struct RenderContext {
	double timeDelta;

	GLuint meshTextureUniformId;
	GLuint meshMVPId;

	GLuint terrainMVPId;
	GLuint terrainMinMaxId;

	glm::mat4 pv;
};

class Renderable {
public:
	virtual void render( const RenderContext &context ) = 0;
};

#endif