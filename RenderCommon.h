static const float RENDER_PI = 3.14159265358979323846264338327950288f;
static const float RENDER_FOV = 45.0f;

typedef unsigned int GLuint;

struct RenderContext {
	double timeDelta;
	GLuint textureUniformId;
};