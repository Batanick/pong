
struct GLFWwindow;

class Renderer final {
public:
	Renderer(GLFWwindow* _window):window(_window){
	}

	void render();

private:
	GLFWwindow * const window;
};