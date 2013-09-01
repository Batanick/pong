#include <memory>

struct GLFWwindow;
class ShaderManager;

class Renderer final {
public:
	Renderer(GLFWwindow* _window):window(_window){
	}

	void render();
	bool init();

private:
	GLFWwindow * const window;
	std::shared_ptr<ShaderManager> shaderManager;
};