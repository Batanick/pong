#include "RenderCommon.h"
#include <vector>
#include <memory>

#include "TerrainConstants.h"
#include "Patches.h"

class Terrain final : public Renderable {

public:
  Terrain() :
    patches(new Patches()) {
    projectionId = -1;
    viewId = -1;
    textureId = -1;
    heightId = -1;
    textureId = -1;
    cameraPosId = -1;
    lightDirId = -1;
  }

  virtual void init(const GLuint shaderId) override;
  virtual void render(const RenderContext &context) override;
  virtual void shutdown() override;
  virtual ShaderType getType() override;

private:
  struct IndexBuffer {
    GLuint id;
    int length;
  };

  void initTexture(const GLuint &shaderId);
  void initVertices(const GLuint &shaderId);
  void initIndices(const GLuint &shaderId);

  std::vector<IndexBuffer> indexBuffers;

  GLuint projectionId;
  GLuint viewId;
  GLuint textureParamId;
  GLuint heightId;
  GLuint cameraPosId;
  GLuint lightDirId;
  GLuint waterLevel;

  GLuint textureId;

  std::unique_ptr<Patches> patches;

  void Terrain::refreshPatch(Patch &patch);
};