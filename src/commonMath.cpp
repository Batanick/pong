#include "commonMath.h"

#include <cstdlib>

static const glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
static const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
static const glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

bool isPower2(int x) {
    return ((x > 0) && ((x & (x - 1)) == 0));
}

float getRandomFloat() {
    return (float) rand() / (float) RAND_MAX;
}

float random(float baseValue, float disp) {
    const float random = ((float) rand()) / (RAND_MAX / 2) - 1;
    return baseValue + (disp * random);
}

glm::quat getRotation(glm::vec3 start, glm::vec3 dest) {
    start = glm::normalize(start);
    dest = glm::normalize(dest);

    float cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(
            s * 0.5f,
            rotationAxis.x * invs,
            rotationAxis.y * invs,
            rotationAxis.z * invs
    );

}

glm::vec3 findPerpendicular(glm::vec3 base) {
    base = glm::normalize(base);
    glm::vec3 result = glm::cross(base, X_AXIS);
    return glm::length(result) < 0.01 ? glm::cross(base, Y_AXIS) : result;
}

inline float interpolate(float a, float b, float x) {
    return a * (1 - x) + b * x;
}

glm::mat3 convertToViewMatrix(float scaleX, float scaleY, int translateX, int translateY) {
    glm::mat3 proj;

    proj[0][0] = scaleX;
    proj[1][1] = scaleY;
    proj[0][2] = (float) translateX;
    proj[1][2] = (float) translateY;

    return proj;
}