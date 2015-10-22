#include "RenderCommon.h"
#include <vector>
#include <memory>
#include <GL/glew.h>

#include "TerrainConstants.h"
#include "Patches.h"

class Terrain final : public Renderable {

public:
    Terrain() :
            patches(new Patches()) {
        projectionId = 0;
        viewId = 0;
        textureId = 0;
        heightId = 0;
        textureId = 0;
        cameraPosId = 0;
        lightDirId = 0;
    }

    virtual void init(const GLuint shaderId) override;

    virtual void shutdown() override;

    virtual ShaderType getType() override;

protected:
    virtual void render(const RenderContext &context) override;

private:
    struct IndexBuffer {
        GLuint id;
        size_t length;
    };

    void initTexture(const GLuint &shaderId);

    void initVertices(const GLuint &shaderId);

    void initIndices(const GLuint &shaderId);

    bool visible(const RenderContext &context, const Patch &patch) const;

    std::vector<IndexBuffer> indexBuffers;

    GLint projectionId;
    GLint viewId;
    GLint textureParamId;
    GLint heightId;
    GLint cameraPosId;
    GLint lightDirId;
    GLint waterLevel;

    GLuint textureId;

    std::unique_ptr<Patches> patches;

    void refreshPatch(Patch &patch);
};