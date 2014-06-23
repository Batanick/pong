#include "TerrainConstants.h"

std::vector< std::pair<float, color>> buildTerrainColors() {
  std::vector< std::pair<float, color>> colors;

  colors.push_back(std::make_pair(0.0f, color(0, 0, 255)));
  colors.push_back(std::make_pair(0.01f, color(0, 0, 255)));

  colors.push_back(std::make_pair(0.05f, color(220, 200, 0)));
  colors.push_back(std::make_pair(0.2f, color(220, 200, 0)));

  colors.push_back(std::make_pair(0.25f, color(80, 40, 0)));
  colors.push_back(std::make_pair(0.30f, color(0, 75, 0)));
  colors.push_back(std::make_pair(0.65f, color(0, 75, 0)));

  colors.push_back(std::make_pair(0.75f, color(80, 80, 80)));
  colors.push_back(std::make_pair(0.85f, color(80, 80, 80)));

  colors.push_back(std::make_pair(1.0f, color(255, 255, 255)));

  return colors;
}
