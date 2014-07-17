#include "RenderCommon.h"

#include <vector>

class TexturedMesh : public Renderable  {
public:
  TexturedMesh() {
    verticesId = -1;
    mvpId = -1;
    textureId = -1;
    indicesId = -1;
  }

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

  virtual void init(const GLuint shaderId) final override;
  virtual void render(const RenderContext &context) final override;
  virtual void shutdown() final override;
  virtual ShaderType getType() final override;

private:
  GLuint verticesId;
  GLuint indicesId; 

  GLuint mvpId;
  GLuint textureId;
  GLuint textureParamId;

  int indicesCount;

  virtual void initVertices(std::vector<const VertexData> &vertices, std::vector<const unsigned short> &indices) = 0;
  virtual void initTexture(GLuint &textureId) = 0;
  virtual glm::mat4 getModelTransform(const RenderContext &context) = 0; 
};