#include "Tree.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx\constants.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>

#include "commonMath.h"
#include "logging.h"

void Tree::initMesh(
  std::vector<const glm::vec3> &vertices,
  std::vector<const unsigned int> &indices,
  std::vector<const glm::vec2> &uvs) {

  StemParams rootParams;
  rootParams.direction = glm::normalize(glm::vec3(0, 1, 0));
  rootParams.curveAxis = glm::normalize(glm::vec3(0, 0, 1));
  rootParams.length = treeParams.rootLength * treeParams.scale;
  rootParams.radius = rootParams.length * treeParams.scale * treeParams.ratio;
  rootParams.resolution = 8;
  rootParams.segments = treeParams.rootCurveRes;
  rootParams.curve = treeParams.rootCurve;
  rootParams.curveBack = treeParams.rootCurveBack;
  rootParams.weist = 1 - treeParams.rootTaper;
  rootParams.pos = glm::vec3(0, 0, 0);

  drawStem(rootParams, vertices, indices, 0, treeParams.baseSize);
}

void Tree::drawStem(const StemParams &stem, std::vector<const glm::vec3> &vertices, std::vector<const unsigned int> &indices, const int level, const float baseSize) {
  const int indicesOffset = vertices.size();
  const float segmentHeight = stem.length / stem.segments;
  const float yawDelta = glm::pi<float>() * 2 / stem.resolution;
  const float radiusWaistFactor = (1 - stem.weist) / stem.segments;
  const float curveAngle = stem.curve / (stem.segments);

  float yaw = 0;
  glm::vec3 pos = stem.pos;
  glm::vec3 increment = glm::normalize(stem.direction) * segmentHeight;

  const TreeLevelParams levelParams = treeParams.getParams(level + 1);
  const float branchesCount = glm::round(levelParams.branches * (0.2f + 0.8f * (levelParams.length / stem.length)));
  const float offsetToLength = stem.length / (1 + baseSize);
  float offset = 0;
  float rotation = 0;
  float segmentOffsetLength = (1 + baseSize) / (stem.segments + 1);
  float offsetPerChild = (branchesCount > 0) ? 1 / branchesCount : 0;
  std::vector<const StemParams> childs;

  for (int row = 0; row <= stem.segments; row++) {
    glm::vec3 rotor = stem.curveAxis * stem.radius * (1 - radiusWaistFactor * row);

    // ================== VERTICES ================
    for (int col = 0; col <= stem.resolution; col++) {
      const glm::vec3 current = pos + glm::vec3(glm::angleAxis(glm::degrees(yaw), glm::normalize(increment)) * rotor);
      vertices.push_back(current);
      yaw += yawDelta;
    }

    // ===============CHILD GENERATION ============
    while ((branchesCount > 0) && (offset < segmentOffsetLength * row)) {
      if (offset < baseSize) {
        offset += glm::min(segmentOffsetLength, baseSize - offset);
        continue;
      }

      const glm::vec3 childPos = pos + (glm::normalize(increment) * (offset * offsetToLength - row * segmentHeight));
      childs.push_back(generateChild(stem, levelParams, childPos, glm::normalize(increment), offset - baseSize, rotation));
      offset += offsetPerChild;
      rotation += levelParams.rotate;
    }

    const float curveAngleActual = glm::abs(stem.curveBack) < 0.01
      ? curveAngle
      : ((row > (stem.segments / 2)) ? (curveAngle * 2) : (stem.curveBack * 2 / stem.segments));

    increment = glm::angleAxis(glm::degrees(curveAngleActual), glm::normalize(stem.curveAxis)) * increment;
    pos += increment;
  }

  // ================== INDICES ================
  for (int row = 0; row < stem.segments; row++) {
    for (int col = 0; col < stem.resolution; col++) {
      indices.push_back(col + row * (stem.resolution + 1) + indicesOffset);
      indices.push_back(col + row * (stem.resolution + 1) + 1 + indicesOffset);
      indices.push_back(col + (row + 1) * (stem.resolution + 1) + indicesOffset);

      indices.push_back(col + row * (stem.resolution + 1) + 1 + indicesOffset);
      indices.push_back(col + (row + 1) * (stem.resolution + 1) + 1 + indicesOffset);
      indices.push_back(col + (row + 1) * (stem.resolution + 1) + indicesOffset);
    }
  }

  for (const auto child : childs) {
    drawStem(child, vertices, indices, level + 1, 0);
  }
}

const Tree::StemParams Tree::generateChild(
  const Tree::StemParams &parentParams,
  const Tree::TreeLevelParams &levelParams,
  const glm::vec3 pos, const glm::vec3 parentDirection,
  const float offsetFactor, const float rotation) {

  const float baseSize = treeParams.baseSize * parentParams.length;

  const float downAngle = random(levelParams.downAngle, levelParams.downAngleV);
  const float offsetLength = parentParams.length - baseSize;

  StemParams childParams;
  childParams.curve = random(levelParams.curve, levelParams.curveV);

  const glm::mat3 rotationMat = glm::toMat3(glm::angleAxis(glm::degrees(rotation), parentDirection) * glm::angleAxis(glm::degrees(downAngle), parentParams.curveAxis));
  childParams.direction = glm::normalize(rotationMat * parentDirection);

  childParams.curveAxis = glm::normalize(glm::cross(childParams.direction, parentDirection));

  const float shapeRatio = treeParams.shape(offsetFactor);
  childParams.length = random(levelParams.length, levelParams.lengthV) * parentParams.length * shapeRatio;
  childParams.pos = pos;
  childParams.radius = parentParams.radius * glm::pow(childParams.length / parentParams.length, treeParams.ratioPower);
  childParams.resolution = parentParams.resolution / 2;
  childParams.segments = levelParams.curveRes;
  childParams.weist = 1 - levelParams.taper;
  childParams.curveBack = levelParams.curveBack;

  return childParams;
}

// ====================================================================================================================
// ===================================== TREE LIBRARY (MAGIC NUMBER KINGDOM) ==========================================
// ====================================================================================================================


float blackTupeloRatio(float ratio) {
  return 0.5f + 0.5f * (1 - ratio);
}

const Tree::TreeParams Tree::blackTupelo() {
  TreeLevelParams level1(glm::radians(60.0f), glm::radians(140.0f), 50, 0.3f, 1.0f, 10, glm::radians(0.0f), glm::radians(0.0f));
  level1.initVars(glm::radians(-40.0f), 0.0f, 0.05f, glm::radians(90.0f));

  TreeLevelParams level2(glm::radians(30.0f), glm::radians(140.0f), 25, 0.6f, 1.0f, 10, glm::radians(-10.0f), glm::radians(0.0f));
  level2.initVars(glm::radians(10.0f), 0.0f, 0.1f, glm::radians(150.0f));

  TreeLevelParams level3(glm::radians(45.0f), glm::radians(140.0f), 12, 0.4f, 1.0f, 1, glm::radians(0.0f), glm::radians(0.0f));
  level3.initVars(glm::radians(10.0f), 0.0f, 0.0f, glm::radians(0.0f));

  std::vector<const TreeLevelParams> levelParams;
  levelParams.push_back(level1);
  levelParams.push_back(level2);
  levelParams.push_back(level3);

  TreeParams params(levelParams, blackTupeloRatio);
  params.ratio = 0.015f;
  params.ratioPower = 1.2f;
  params.rootLength = 1.0f;
  params.rootCurveRes = 10;
  params.rootCurve = 0.0f;
  params.rootCurveBack = 0.0f;
  params.scale = 1.0f;
  params.rootTaper = 1.0f;
  params.baseSize = 0.4f;

  return params;
}

float blackOakRatio(float offsetFactor) {
  return 0.2f + 0.8f * glm::sin(0.5f * glm::pi<float>() * (1 - offsetFactor));
}

const Tree::TreeParams Tree::blackOak() {
  TreeLevelParams level1(glm::radians(30.0f), glm::radians(80.0f), 40, 0.8f, 1.0f, 10, glm::radians(40.0f), glm::radians(-70.0f));
  level1.initVars(glm::radians(-30.0f), 0.0f, 0.01f, glm::radians(150.0f));

  TreeLevelParams level2(glm::radians(45.0f), glm::radians(140.0f), 120, 0.2f, 1.0f, 3, glm::radians(0.0f), glm::radians(-30.0f));
  level2.initVars(glm::radians(10.0f), 0.0f, 0.05f, glm::radians(-30.0f));

  TreeLevelParams level3(glm::radians(45.0f), glm::radians(140.0f), 0, 0.4f, 1.0f, 1, glm::radians(0.0f), glm::radians(0.0f));
  level3.initVars(glm::radians(10.0f), 0.0f, 0.0f, glm::radians(0.0f));

  std::vector<const TreeLevelParams> levelParams;
  levelParams.push_back(level1);
  levelParams.push_back(level2);
  levelParams.push_back(level3);

  TreeParams params(levelParams, blackOakRatio);
  params.ratio = 0.015f;
  params.ratioPower = 1.2f;
  params.rootLength = 1.0f;
  params.rootCurveRes = 10;
  params.rootCurve = 0.0f;
  params.rootCurveBack = 0.0f;
  params.scale = 1.0f;
  params.rootTaper = 1.0f;
  params.baseSize = 0.4f;

  return params;
}


