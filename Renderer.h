#include "RenderCommon.h"

#include <memory>
#include <vector>

struct GLFWwindow;
struct RenderContext;

class ShaderManager;
class AssetManager;

class Mesh;
class Terrain;
class Camera;
class Tickable;
class FpsCounter;

typedef std::shared_ptr<Renderable> PRenderable;

class Renderer final {
public:
	Renderer(GLFWwindow* _window):window(_window){
	}

	void render( double timeDelta );
	bool init();
	void shutdown();
	
	std::shared_ptr<Tickable> getTickable(); 

private:
	GLFWwindow* const window;
    
	std::shared_ptr<FpsCounter> fpsCounter; 

	std::shared_ptr<ShaderManager> shaderManager;
	std::shared_ptr<AssetManager> assetManager;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Terrain> terrain;

	std::vector<PRenderable> renderables; 

	RenderContext context;

	void initContext();

	void renderMeshes();
	void renderTerrain();
	void renderTexts();
};

