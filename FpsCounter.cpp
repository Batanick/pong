#include "FpsCounter.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging.h"

static double const SECOND = 1;

double FpsCounter::onFrame() {
	const double currentTime = glfwGetTime();
	fpsCounter++;
		
	const double timeDelta = currentTime - lastTickTime;
	timeCounter += timeDelta;
	if ( timeCounter > SECOND ) {
		currentFps = (fpsCounter) / timeCounter;
        
        fpsCounter = 0;
		timeCounter = 0;

		LOG ( "FPS: %f", currentFps );
	}

	lastTickTime = currentTime;
	return timeDelta;
}

double FpsCounter::getFps() {
	return currentFps;
}