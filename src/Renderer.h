#include "RenderCommon.h"

#include <memory>
#include <vector>
#include <map>

#include "logging.h"

struct GLFWwindow;
struct RenderContext;

class AssetManager;

class Terrain;

class Camera;

class Tickable;

class FpsCounter;

class Label;

class ShaderManager;

enum class ShaderType : unsigned int;


class RenderHandler;

class Renderer final : public CommonRenderer {
public:
    Renderer(GLFWwindow *_window) : window(_window) {
        renderables.resize(3); // Renderable types count
    }

    void render(double timeDelta);

    bool init();

    void shutdown();

    virtual void renderAll() override;

    virtual void render(const RenderableType &type, const RenderContext &context) override;

    virtual void add(PRenderable renderable, RenderableType type = RenderableType::Common) override;

private:
    typedef std::multimap<ShaderType, PRenderable> RenderableHolder;
    typedef std::shared_ptr<RenderHandler> PRenderHandler;

    GLFWwindow *const window;

    std::shared_ptr<FpsCounter> fpsCounter;
    std::shared_ptr<Label> statsLabel;

    std::shared_ptr<ShaderManager> shaderManager;
    std::shared_ptr<AssetManager> assetManager;
    std::shared_ptr<Camera> camera;

    std::vector<RenderableHolder> renderables;
    std::vector<PRenderHandler> addons;

    RenderContext context;

    void initContext();

    void initScene();
};

