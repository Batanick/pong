#include <memory>

struct GLFWwindow;
class Renderer;

class Game final {
private:
	bool running;
	GLFWwindow* window;
	std::shared_ptr<Renderer> renderer;

	void runMainLoop();

public:
	Game():running(false) {
	};

	bool init();
	void start();
	void shutdown();
};