#include "glm.hpp"
#include "gtx\quaternion.hpp"

bool isPower2(int x);

template <class T>
T trim(T min, T max, T value) {
  if (value < min) {
    return min;
  }

  return value > max ? max : value;
}

glm::quat getRotation(glm::vec3 start, glm::vec3 dest);

glm::vec3 findPerpendicular(glm::vec3 vector);

float getRandomFloat();

float random(float baseValue, float disp);

inline float interpolate(float a, float b, float x);

glm::mat4 getViewMatrix(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up);