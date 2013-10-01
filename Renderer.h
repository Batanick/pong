#include "RenderCommon.h"

#include <memory>
#include <vector>

struct GLFWwindow;
struct RenderContext;

class ShaderManager;
class Mesh;
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
	GLFWwindow * const window;
	std::shared_ptr<ShaderManager> shaderManager;
	std::shared_ptr<Camera> camera;
	std::vector<PMesh> meshes; 

	RenderContext context;
};

