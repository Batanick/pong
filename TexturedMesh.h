#include "RenderCommon.h"

#include <vector>

class TexturedMesh final : public Renderable  {
public:
  TexturedMesh() {
    verticesId = -1;
    mvpId = -1;
    textureId = -1;
    indicesId = -1;
  }

  virtual void init(const GLuint shaderId) override;
  virtual void render(const RenderContext &context) override;
  virtual void shutdown() override;

private:
  struct VertexData {
    VertexData(const glm::vec3 &position, const glm::vec2 uv) {
      this->position = position;
      this->uv = uv;
    }

    VertexData() {
    }

    glm::vec3 position;
    glm::vec2 uv;
  };

  GLuint verticesId;
  GLuint indicesId; 

  GLuint mvpId;
  GLuint textureId;
  GLuint textureParamId;

  int indicesCount;

  void initVertices(std::vector<const VertexData> &vertices, std::vector<const unsigned short> &indices);
  void initTexture(GLuint &textureId);
  glm::mat4 getModelTransform(const RenderContext &context); 
};