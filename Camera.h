#include <memory>
#include <glm.hpp>

struct RenderContext;

class Camera final {

public:
	Camera();

	void onBeforeRender( const RenderContext &context );
	glm::mat4 getProjection();
	glm::mat4 getView();

private:
	float horizontalAngle;
	float verticalAngle;
	glm::vec3 position; 
};