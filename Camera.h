#include <memory>
#include <glm.hpp>

struct RenderContext;
struct GLFWwindow;

class Camera final {

public:
	Camera();

	void Camera::onBeforeRender( GLFWwindow * const window, double timeDelta );
	glm::mat4 Camera::getView();
	glm::mat4 Camera::getProjection();

private:
	float horizontalAngle;
	float verticalAngle;
	glm::vec3 position; 

	glm::vec3 getRight();
	glm::vec3 getDirection();
};