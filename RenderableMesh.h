#ifndef H_RENDERABLE
#define H_RENDERABLE

#include <vector>

#include "glm.hpp"

#include "RenderCommon.h"

class RenderableMesh: public Renderable {
public:
  virtual ~RenderableMesh() {
    //nothing here
  }

  virtual void render(const RenderContext &context) final override;
  virtual void shutdown() final override;
  virtual void init(const GLuint shaderId) final override;

protected:
  virtual void initMesh(
    std::vector<const VertexData> &vertices,
    std::vector<const unsigned int> &indices) = 0;

  void setColor(float r, float g, float b);

private:

  GLuint vertexBuffer;
  GLuint indexBuffer;

  GLuint projectionId;
  GLuint viewId;
  GLuint lightDirId;
  GLuint colorId;

  glm::vec3 color;

  int indicesSize;

};

#endif