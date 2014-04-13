#include "Camera.h"

#include <GLFW/glfw3.h>
#include <gtc\matrix_transform.hpp>

#include "RenderCommon.h"

#include "logging.h"

static const float CAMERA_MOVE_SPEED = 3.0f; 
static const float CAMERA_ROTATION_SPEED = 0.005f;

Camera::Camera() {
	horizontalAngle = 0.78f;
	verticalAngle = -0.2f;
	position = glm::vec3( 0, 1, 0 );
}

void Camera::onBeforeRender( GLFWwindow * const window, double deltaTime ) {
	if ( glfwGetWindowAttrib( window, GLFW_FOCUSED ) != GL_TRUE ) {
		return;
	}

	double mouseXPos, mouseYPos;
	glfwGetCursorPos( window, &mouseXPos, &mouseYPos );
	int windowHeight, windowWidth;
	glfwGetWindowSize( window, &windowWidth, &windowHeight );
    projection = glm::perspective(45.0f, ((float)windowWidth) / windowHeight, 0.1f, 100.0f);

	glfwSetCursorPos(window, windowWidth/2, windowHeight/2);

	horizontalAngle += CAMERA_ROTATION_SPEED * float( windowWidth/2 - mouseXPos );
	verticalAngle   += CAMERA_ROTATION_SPEED * float( windowHeight/2 - mouseYPos );
	
	const glm::vec3 direction = getDirection();
	const glm::vec3 right = getRight();
	
	// Forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * (float)deltaTime * CAMERA_MOVE_SPEED;
	}
	// Backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * (float)deltaTime * CAMERA_MOVE_SPEED;
	}
	// Right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * (float)deltaTime * CAMERA_MOVE_SPEED;
	}
	// Left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * (float)deltaTime * CAMERA_MOVE_SPEED;
	}
	
	//Switching polygon draw mode
	if (glfwGetKey( window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		GLint currentMode[2]; //front and back value
		glGetIntegerv( GL_POLYGON_MODE, &currentMode[0] );
		glPolygonMode( GL_FRONT_AND_BACK,  currentMode[0] == GL_LINE ? GL_FILL : GL_LINE );
	}
}

glm::vec3 Camera::getRight() {
	return glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
}

glm::vec3 Camera::getDirection() {
	return glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::mat4 Camera::getView() {
	const glm::vec3 direction = getDirection();
	const glm::vec3 up = glm::cross( getRight(), direction );
	return glm::lookAt( position, position+direction, up);
}

glm::mat4 Camera::getProjection() {
    return projection;
}