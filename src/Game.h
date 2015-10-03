#include <memory>
#include <vector>
#include <mutex>

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

    bool isRunning();

private:
    bool running;

    GLFWwindow *window;
    double lastTick;

    std::shared_ptr<Renderer> renderer;
    std::vector<TickablePtr> tickables;

    void runMainLoop();

    void onShutdown();
};