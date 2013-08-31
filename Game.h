
struct GLFWwindow;

class Game {
private:
	bool running;
	GLFWwindow* window;

	void runMainLoop();

public:
	Game():running(false) {
	};

	bool init();
	void start();
	void shutdown();
};