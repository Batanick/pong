#include <memory>
#include <vector>

#include "Tickable.hpp"

struct GLFWwindow;

struct RenderContext;
class Renderer;


class Game final {

public:
	Game();

	bool init();
	void start();
	void shutdown();
	
private:
	bool running;
	
	GLFWwindow* window;
	double lastTick;

	std::shared_ptr<Renderer> renderer;
	std::vector<TickablePtr> tickables;
	
	class Timer;
	std::shared_ptr<Timer> timer; 

	void runMainLoop();
	RenderContext createRenderContext();
};