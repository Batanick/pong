#include "RenderCommon.h"

#include <vector>
#include <GL/gl.h>

class TexturedMesh : public Renderable {
public:
    TexturedMesh() {
        verticesId = 0;
        mvpId = 0;
        textureId = 0;
        indicesId = 0;
    }

    struct VertexData {
        VertexData(const glm::vec3 &position, const glm::vec2 &uv) : position(position), uv(uv) { }

        VertexData() {
        }

        glm::vec3 position;
        glm::vec2 uv;
    };

    virtual void init(const GLuint shaderId) final;

    virtual void render(const RenderContext &context);

    virtual void shutdown() final;

    virtual ShaderType getType() final;

private:
    GLuint verticesId;
    GLuint indicesId;

    GLint mvpId;
    GLuint textureId;
    GLint textureParamId;

    size_t indicesCount;

    virtual void initVertices(std::vector<VertexData> &vertices, std::vector<unsigned short> &indices) = 0;

    virtual void initTexture(GLuint &textureId) = 0;

    virtual glm::mat4 getModelTransform(const RenderContext &context) = 0;
};