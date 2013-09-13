#include "Camera.h"

#include <GLFW/glfw3.h>
#include <gtc\matrix_transform.hpp>

#include "RenderCommon.h"

#include "logging.h"

static const float CAMERA_MOVE_SPEED = 3.0f; 
static const float CAMERA_ROTATION_SPEED = 0.005f;

Camera::Camera() {
	horizontalAngle = 3.14f;
	verticalAngle = 0;
	position = glm::vec3( 0, 0, 5 );
}

glm::mat4 Camera::getProjection() {
	static glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	return projection;
}

glm::mat4 Camera::getView() {
	const glm::vec3 direction = getDirection();
	const glm::vec3 up = glm::cross( getRight(), direction );
	glm::mat4 view =   glm::lookAt( position, position+direction, up);
	return view;
}


void Camera::onBeforeRender( const RenderContext &context ) {
	horizontalAngle += CAMERA_ROTATION_SPEED * float( context.windowWidth/2 - context.mouseXPos );
	verticalAngle   += CAMERA_ROTATION_SPEED * float( context.windowHeight/2 - context.mouseYPos );
	
	const glm::vec3 direction = getDirection();
	const glm::vec3 right = getRight();
	
	const float deltaTime = (float) context.timeDelta;
	
	// Forward
	if (glfwGetKey( context.window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * CAMERA_MOVE_SPEED;
	}
	// Backward
	if (glfwGetKey( context.window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * CAMERA_MOVE_SPEED;
	}
	// Right
	if (glfwGetKey( context.window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * CAMERA_MOVE_SPEED;
	}
	// Left
	if (glfwGetKey( context.window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * CAMERA_MOVE_SPEED;
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