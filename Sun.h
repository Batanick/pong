#include "TexturedMesh.h"

static const float MESH_SIZE = 128;
static const float MESH_HEIGHT = 1000;

class Sun final : public TexturedMesh {
public:
  Sun():TexturedMesh() {
    //nothing here
  }

private:
  virtual void initVertices(std::vector<const TexturedMesh::VertexData> &vertices, std::vector<const unsigned short> &indices) override;
  virtual void initTexture(GLuint &textureId) override;
  virtual glm::mat4 getModelTransform(const RenderContext &context) override;
};