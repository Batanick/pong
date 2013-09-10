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
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	glm::vec3 up = glm::cross( right, direction );

	glm::mat4 view =   glm::lookAt(
				position,           // Camera is here
				position+direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	return view;
}


void Camera::onBeforeRender( const RenderContext &context ) {
	// Reset mouse position for next frame

	// Compute new orientation
	horizontalAngle += CAMERA_ROTATION_SPEED * float( context.windowWidth/2 - context.mouseXPos );
	verticalAngle   += CAMERA_ROTATION_SPEED * float( context.windowHeight/2 - context.mouseYPos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	const float deltaTime = (float) context.timeDelta;
	// Move forward
	if (glfwGetKey( context.window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * CAMERA_MOVE_SPEED;
	}
	// Move backward
	if (glfwGetKey( context.window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * CAMERA_MOVE_SPEED;
	}
	//// Strafe right
	if (glfwGetKey( context.window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * CAMERA_MOVE_SPEED;
	}
	// Strafe left
	if (glfwGetKey( context.window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * CAMERA_MOVE_SPEED;
	}
	
}

