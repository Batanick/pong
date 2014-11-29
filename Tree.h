#include "renderCommon.h"
#include "RenderableMesh.h"

#include <vector>

class Tree final : public RenderableMesh {
public:
  Tree() :
    pos(glm::vec3()),
    treeParams(blackOak()) {
  }

  virtual ~Tree() {
    // nothing 
  }

  virtual ShaderType getType() {
    return ShaderType::BUSH_SHADER;
  }

protected:
  virtual void initMesh(MeshContext &mesh) override;
  virtual void Tree::initTexture(GLuint &textureId) override;
  virtual RenderableMesh::TriangleMode getTrianglesMode() override;

private:
  struct TreeLevelParams {
    TreeLevelParams(float downAngle, float rotate, int branches, float length,
      float taper, int curveRes, float curve, float curveBack) :
      downAngle(downAngle),
      rotate(rotate),
      branches(branches),
      length(length),
      taper(taper),
      curveRes(curveRes),
      curve(curve),
      curveBack(curveBack) {
      downAngleV = 0;
      rotationV = 0;
      lengthV = 0;
      curveV = 0;
    }

    TreeLevelParams() {
      downAngle = 0.0f;
      downAngleV = 0.0f;
      rotate = 0.0f;
      rotationV = 0.0f;
      branches = 0;
      length = 0.0f;
      lengthV = 0.0f;
      taper = 0.0f;
      curveRes = 1;
      curve = 0.0f;
      curveV = 0.0f;
      curveBack = 0.0f;
    }

    void initVars(
      const float downAngleV,
      const float rotationV,
      const float lengthV,
      const float curveV) {
      this->downAngleV = downAngleV;
      this->rotationV = rotationV;
      this->lengthV = lengthV;
      this->curveV = curveV;
    }

    float downAngle, downAngleV;
    float rotate, rotationV;
    int branches;
    float length, lengthV;
    float taper;
    int curveRes;
    float curve, curveV;
    float curveBack;
  };

  struct TreeParams {
    typedef float(*shapeFunction)(float ration);

    TreeParams(std::vector<const TreeLevelParams> levelParams, shapeFunction shape) :
      levelsList(levelParams),
      shape(shape){
    }

    shapeFunction shape;
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
    unsigned char segments;
    unsigned char resolution;
    float curve;
    float curveBack;
    float radius;
    float length;
    float weist;
  };

  const TreeParams treeParams;
  const glm::vec3 pos;

  void drawStem(const StemParams &stem, const int level, const float baseSize, MeshContext &mesh);

  const StemParams generateChild(
    const StemParams &parentParams, const TreeLevelParams &levelParams,
    const glm::vec3 pos, const glm::vec3 parentDirection,
    const float offsetFactor, const float rotation);

  static const TreeParams blackTupelo();
  static const TreeParams blackOak();
  static const TreeParams testCone();

  unsigned int indicesCount;
};