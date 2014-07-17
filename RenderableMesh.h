#ifndef H_RENDERABLE
#define H_RENDERABLE

#include <vector>

#include "glm.hpp"

#include "RenderCommon.h"

class RenderableMesh: public Renderable {
public:
  virtual void render(const RenderContext &context) final override;
  virtual void shutdown() final override;
  virtual void init(const GLuint shaderId) final override;

protected:
  virtual void initMesh(
    std::vector<const glm::vec3> &vertices,
    std::vector<const unsigned int> &indices,
    std::vector<const glm::vec2> &uvs) = 0;

  void setColor(float r, float g, float b);

private:

  GLuint vertexBuffer;
  GLuint indexBuffer;

  GLuint mvpId;

  glm::vec3 color;

  int indicesSize;

};

#endif