#include "glm.hpp"
#include "gtx\quaternion.hpp"

bool isPower2(int x);

int trim( int min, int max, int value);

glm::quat getRotation( glm::vec3 start, glm::vec3 dest );

glm::vec3 findPerpendicular( glm::vec3  vector );

float getRandomFloat();