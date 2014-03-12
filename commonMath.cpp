#include "commonMath.h"

#include <cstdlib>

bool isPower2(int x) {
	return ( (x > 0) && ((x & (x - 1)) == 0) );
}

int trim( int min, int max, int value) {
	if (value < min) {
		return min;
	} 

	return value > max ? max : value;
}

float getRandomFloat() {
	return (float)rand()/(float)RAND_MAX;
}

glm::quat getRotation( glm::vec3 start, glm::vec3 dest ){
    start = glm::normalize(start);
    dest = glm::normalize(dest);
 
    float cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;
 
    if ( cosTheta < -1 + 0.001f ){
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = glm::cross( glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross( glm::vec3(1.0f, 0.0f, 0.0f), start );
 
        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);
    }
 
    rotationAxis = glm::cross(start, dest);
 
    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;
 
    return glm::quat(
        s * 0.5f, 
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
 
}