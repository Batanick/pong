#include "RenderCommon.h"

#include <memory>
#include <vector>

struct GLFWwindow;
struct RenderContext;

class ShaderManager;
class TextureManager;

class Mesh;
class Terrain;
class Camera;
class Tickable;

typedef std::shared_ptr<Mesh> PMesh;

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
	
	std::shared_ptr<ShaderManager> shaderManager;
	std::shared_ptr<TextureManager> textureManager;

	std::shared_ptr<Camera> camera;
	std::vector<PMesh> meshes; 
	std::shared_ptr<Terrain> terrain;

	RenderContext context;

	void renderMeshes();
	void renderTerrain();
};

