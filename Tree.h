#include "renderCommon.h"

#include <vector>

class Tree : public Renderable {
public:
    Tree(): 
        pos(glm::vec3()),
        treeParams(getParams()) {
    }

    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

    void init();

private:
    struct TreeLevelParams {
       TreeLevelParams( float downAngle, float rotate, int branches, float length, 
                       float taper, int curveRes, float curve, float curveBack): 
            downAngle(downAngle),
            rotate(rotate), 
            branches(branches),
            length(length),
            taper(taper), 
            curveRes(curveRes),
            curve(curve),
            curveBack(curveBack){
        }

        TreeLevelParams() {
        }

        float downAngle;
        float rotate;
        int branches;
        float length;
        float taper;
        int curveRes;
        float curve;
        float curveBack;
    };

    struct TreeParams {
        TreeParams( std::vector<const TreeLevelParams> levelParams ):
            levelsList(levelParams) {
        }

        float ratio;
        float ratioPower;
        float rootLength;
        int rootCurveRes;
        float rootCurve;
        float rootCurveBack;
        float rootTaper;
        float scale;
        float baseSize;
        
        const std::vector<const TreeLevelParams> levelsList;

        const TreeLevelParams getParams(unsigned int level) const {
            return levelsList.size() > (level - 1) ? levelsList[level - 1] : TreeLevelParams();
        }
    };

    struct StemParams {
        glm::vec3 direction;
        glm::vec3 curveAxis;
        glm::vec3 pos;
        int segments;
        int resolution;
        float curve;
        float radius;
        float length;
        float weist;
    };

    const TreeParams treeParams;
    const glm::vec3 pos;
    void drawStem( const StemParams &stem, std::vector<const glm::vec3> &vertices, std::vector<unsigned int> &indices, const int level, const float baseSize );

    const StemParams generateChild( 
        const StemParams &parentParams, const TreeLevelParams &levelParams, 
        const glm::vec3 pos, const glm::vec3 parentDirection, 
        const float offsetFactor, const float rotation );

    static const TreeParams getParams();

    unsigned int indicesCount;

    GLuint vertexBuffer;
	GLuint indexBuffer;
};