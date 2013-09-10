static const float RENDER_FOV = 45.0f;

struct GLFWwindow;

struct RenderContext {
	GLFWwindow * window;

	double timeDelta;

	double mouseXPos;
	double mouseYPos;

	int windowHeight;
	int windowWidth;
};