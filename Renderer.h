
struct GLFWwindow;

class Renderer final {
public:
	Renderer(GLFWwindow* _window):window(_window){
	}

	void render();
	bool init();

private:
	GLFWwindow * const window;
};