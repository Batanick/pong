#include <memory>
#include <glm/glm.hpp>

struct RenderContext;
struct GLFWwindow;

class Camera final {

public:
    Camera();

    void onBeforeRender(GLFWwindow *const window, double timeDelta);

    glm::vec3 getPosition();

    glm::vec3 getRight();

    glm::vec3 getDirection();

    glm::vec3 getUp();

private:
    float horizontalAngle;
    float verticalAngle;
    glm::vec3 position;
};