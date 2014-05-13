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
enum class ShaderType: unsigned int;

typedef std::shared_ptr<Renderable> PRenderable;

class Renderer final {
public:
	Renderer(GLFWwindow* _window):window(_window) {
	}

	void render( double timeDelta );
	void refreshSome();
	bool init();
	void shutdown();
	
	std::shared_ptr<Tickable> getTickable(); 

    void add( ShaderType type, PRenderable renderable );

private:
	GLFWwindow * const window;
    
	std::shared_ptr<FpsCounter> fpsCounter; 
    std::shared_ptr<Label> fpsLabel;
    std::shared_ptr<Label> cameraCoords;

	std::shared_ptr<ShaderManager> shaderManager;
	std::shared_ptr<AssetManager> assetManager;
	std::shared_ptr<Camera> camera;

    std::multimap<ShaderType, PRenderable> renderables;

	RenderContext context;

	void initContext();
    void initScene();

	void renderAll();
};

