#include "RenderCommon.h"

#include <memory>
#include <vector>
#include <map>

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

typedef std::shared_ptr<Renderable> PRenderable;

class Renderer final {
public:
  Renderer(GLFWwindow* _window) :window(_window) {
    renderables.resize(3); // Renderable types count
  }

  void render(double timeDelta);
  bool init();
  void shutdown();

private:
  enum RenderableType : unsigned char {
    Common = 0, PostRender, GUI
  };

  typedef std::multimap<ShaderType, PRenderable> RenderableHolder;

  GLFWwindow * const window;

  std::shared_ptr<FpsCounter> fpsCounter;
  std::shared_ptr<Label> fpsLabel;
  std::shared_ptr<Label> cameraCoordsLabel;

  std::shared_ptr<ShaderManager> shaderManager;
  std::shared_ptr<AssetManager> assetManager;
  std::shared_ptr<Camera> camera;

  std::vector<RenderableHolder> renderables;

  RenderContext context;

  void initContext();
  void initScene();

  void renderAll();

  void add(ShaderType shaderType, PRenderable renderable, Renderer::RenderableType type = RenderableType::Common);
};

