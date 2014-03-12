#include "renderCommon.h"

#include <vector>

class Tree : public Renderable {
public:
    Tree(): 
        pos(glm::vec3()) {
    }

    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

    void init();

private:
    struct StemParams {
        glm::vec3 direction;
        glm::vec3 rotationAxis;
        glm::vec3 pos;
        int segments;
        int resolution;
        float curve;
        float radius;
        float length;
        float weist;
        float baseSize;
        int maxBranches;
    };

    const glm::vec3 pos;
    void drawStem( const StemParams stem, std::vector<const glm::vec3> &vertices, std::vector<unsigned int> &indices );

    unsigned int indicesCount;

    GLuint vertexBuffer;
	GLuint indexBuffer;
};