#include <memory>
#include <glm.hpp>

struct RenderContext;
struct GLFWwindow;

class Camera final {

public:
  Camera();

  void Camera::onBeforeRender(GLFWwindow * const window, double timeDelta);
  glm::mat4 Camera::getView();
  glm::mat4 Camera::getProjection();
  glm::vec3 getPosition();

private:
  float horizontalAngle;
  float verticalAngle;
  glm::vec3 position;
  glm::mat4 projection;

  glm::vec3 getRight();
  glm::vec3 getDirection();
};