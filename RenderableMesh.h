#ifndef H_RENDERABLE
#define H_RENDERABLE

#include <vector>

#include "glm.hpp"

#include "RenderCommon.h"

class RenderableMesh : public Renderable {
public:
  virtual ~RenderableMesh() {
    textureId = -1;
    textureParamId = -1;
    lightDirId = -1;
    viewId = -1;
    projectionId = -1;
  }

  virtual void render(const RenderContext &context) final override;
  virtual void shutdown() final override;
  virtual void init(const GLuint shaderId) final override;

protected:
  typedef unsigned int TriangleMode;

  struct MeshContext
  {
    std::vector<const TexVertexData> vertices;
    std::vector<const unsigned int> indices;
  };

  virtual void initMesh( MeshContext &mesh ) = 0;
  virtual void initTexture(GLuint &textureId) = 0;
  virtual TriangleMode getTrianglesMode();

private:

  GLuint vertexBuffer;
  GLuint indexBuffer;

  GLuint projectionId;
  GLuint viewId;
  GLuint lightDirId;
  GLuint textureId;
  GLuint textureParamId;

  int indicesSize;
};

#endif