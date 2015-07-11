#include "Tree.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx\constants.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>

#include "commonMath.h"
#include "renderUtils.h"
#include "logging.h"

Tree::Tree(glm::vec3 pos) :
        world(glm::translate(glm::mat4(), pos)), treeParams(blackTupelo()) {
    // nothing
}

void Tree::initMesh(MeshContext & mesh) {
    StemParams rootParams;
    rootParams.direction = glm::normalize(glm::vec3(0, 1, 0));
    rootParams.curveAxis = glm::normalize(glm::vec3(0, 0, 1));
    rootParams.length = treeParams.rootLength * treeParams.scale;
    rootParams.radius = rootParams.length * treeParams.scale * treeParams.ratio;
    rootParams.resolution = 8;
    rootParams.segments = treeParams.rootCurveRes;
    rootParams.curve = treeParams.rootCurve;
    rootParams.curveBack = treeParams.rootCurveBack;
    rootParams.weist = 1 - treeParams.rootTaper;
    rootParams.pos = glm::vec3(0, 0, 0);

    drawStem(rootParams, 0, treeParams.baseSize, mesh);
}

Tree::StemDrawingContext Tree::buildContext(const StemParams &stem, const TreeLevelParams &levelParams,
                                            const float &baseSize) {
    StemDrawingContext context;
    context.segmentHeight = stem.length / stem.segments;
    context.yawDelta = glm::pi<float>() * 2 / stem.resolution;
    context.radiusWaistFactor = (1 - stem.weist) / stem.segments;
    context.curveAngle = stem.curve / stem.segments;
    context.branchesCount = glm::round(levelParams.branches * (0.2f + 0.8f * (levelParams.length / stem.length)));
    context.offsetToLength = stem.length / (1 + baseSize);
    context.offsetPerChild = (context.branchesCount > 0) ? 1.0f / levelParams.branches : 0.0f;
    context.segmentOffsetLength = (1 + baseSize) / (stem.segments + 1);

    return context;
}

void Tree::drawStem(const StemParams &stem, const unsigned int level, const float baseSize, MeshContext &mesh) {
    const TreeLevelParams levelParams = treeParams.getParams(level + 1);
    const StemDrawingContext context = buildContext(stem, levelParams, baseSize);
    const int indicesOffset = mesh.vertices.size();

    float yaw = 0;
    glm::vec3 startPos = stem.pos;
    glm::vec3 increment = glm::normalize(stem.direction) * context.segmentHeight;
    float offset = 0;
    float rotation = 0;

    std::vector<const StemParams> childs;

    for (unsigned char row = 0; row <= stem.segments; row++) {
        // ================== VERTICES ================
        for (unsigned char col = 0; col <= stem.resolution; col++) {
            const glm::vec3 addition = glm::normalize(
                    glm::vec3(glm::angleAxis(glm::degrees(yaw), glm::normalize(increment)) * stem.curveAxis));
            const glm::vec3 current = startPos + (addition * stem.radius * (1 - context.radiusWaistFactor * row));

            mesh.vertices.push_back(TexVertexData(current, addition, glm::vec2(0.25f, 0.25f)));
            yaw += context.yawDelta;
        }

        // ===============CHILD GENERATION ============
        while ((context.branchesCount > 0) && (offset < context.segmentOffsetLength * row)) {
            if (offset < baseSize) {
                offset += glm::min(context.segmentOffsetLength, baseSize - offset);
                continue;
            }

            const glm::vec3 childPos = startPos + (glm::normalize(increment) *
                                                   (offset * context.offsetToLength - row * context.segmentHeight));
            childs.push_back(
                    generateChild(stem, levelParams, childPos, glm::normalize(increment), offset - baseSize, rotation));
            offset += context.offsetPerChild;
            rotation += levelParams.rotate;
        }

        const float curveAngleActual = glm::abs(stem.curveBack) < 0.01
                                       ? context.curveAngle
                                       : ((row > (stem.segments / 2)) ? (context.curveAngle * 2) : (stem.curveBack * 2 /
                                                                                                    stem.segments));

        increment = glm::angleAxis(glm::degrees(curveAngleActual), glm::normalize(stem.curveAxis)) * increment;
        startPos += increment;
    }

    // ================== INDICES ================
    generateTriangleIndices(indicesOffset, stem.segments, stem.resolution, mesh.indices);

    for (const auto child : childs) {
        if (treeParams.levelsList.size() != level + 1) {
            drawStem(child, level + 1, 0, mesh);
        }
        else {
            drawLeaf(child, level + 1, 0, mesh);
        }
    }
}

void Tree::drawLeaf(const StemParams &stem, unsigned const int level, const float baseSize, MeshContext &mesh) {
    const int indicesOffset = mesh.vertices.size();
    const glm::vec3 widthVec =
            glm::normalize(stem.curveAxis) * 0.5f * stem.length * (1 - treeParams.leafStem) * treeParams.leafScale *
            treeParams.leafForm;
    const glm::vec3 heightVec =
            glm::normalize(stem.direction) * stem.length * (1 - treeParams.leafStem) * treeParams.leafScale;
    const glm::vec3 normal = glm::normalize(glm::cross(widthVec, heightVec));
    const glm::vec3 leafStartPos = stem.pos + stem.direction * stem.length * treeParams.leafStem;

    //TODO fix normals for back side
    mesh.vertices.push_back(TexVertexData(leafStartPos + widthVec + heightVec, normal, glm::vec2(1.0f, 1.0f)));
    mesh.vertices.push_back(TexVertexData(leafStartPos - widthVec + heightVec, normal, glm::vec2(1.0f, 0.5f)));
    mesh.vertices.push_back(TexVertexData(leafStartPos - widthVec, normal, glm::vec2(0.0f, 0.5f)));
    mesh.vertices.push_back(TexVertexData(leafStartPos + widthVec, normal, glm::vec2(0.0f, 1.0f)));

    generateQuadIndices(mesh.indices, indicesOffset, true);
}

const Tree::StemParams Tree::generateChild(
        const Tree::StemParams &parentParams,
        const Tree::TreeLevelParams &levelParams,
        const glm::vec3 pos, const glm::vec3 parentDirection,
        const float offsetFactor, const float rotation) {

    const float baseSize = treeParams.baseSize * parentParams.length;

    const float downAngle = random(levelParams.downAngle, levelParams.downAngleV);
    const float offsetLength = parentParams.length - baseSize;

    StemParams childParams;
    childParams.curve = random(levelParams.curve, levelParams.curveV);

    const glm::mat3 rotationMat = glm::toMat3(glm::angleAxis(glm::degrees(rotation), parentDirection) *
                                              glm::angleAxis(glm::degrees(downAngle), parentParams.curveAxis));
    childParams.direction = glm::normalize(rotationMat * parentDirection);

    childParams.curveAxis = glm::normalize(glm::cross(childParams.direction, parentDirection));

    const float shapeRatio = treeParams.shape(offsetFactor);
    childParams.length = random(levelParams.length, levelParams.lengthV) * parentParams.length * shapeRatio;
    childParams.pos = pos;
    childParams.radius =
            parentParams.radius * glm::pow(childParams.length / parentParams.length, treeParams.ratioPower);
    childParams.resolution = parentParams.resolution / 2;
    childParams.segments = levelParams.curveRes;
    childParams.weist = 1 - levelParams.taper;
    childParams.curveBack = levelParams.curveBack;

    return childParams;
}

void Tree::initTexture(GLuint &textureId) {
    static const int texSegments = 2;

    static const int texWidth = 256;
    static const int texSegmentHeight = 256;

    static const int textureSize = texWidth * texSegmentHeight * texSegments;

    std::vector<unsigned char> texData;

    // branch texture
    for (int y = 0; y < texSegmentHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            texData.push_back(128u);              // R
            texData.push_back(64u);              // G
            texData.push_back(0u);              // B
            texData.push_back(255u);              // A
        }
    }

    // leaf texture
    static const int centerX = texWidth / 2;
    static const int centerY = texSegmentHeight / 2;
    for (int y = 0; y < texSegmentHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            const double distance = sqrt((x - centerX) * (x - centerX) + (y - centerX) * (y - centerX)) / centerX;

            texData.push_back(0u);              // R
            texData.push_back(200u);              // G
            texData.push_back(0u);              // B
            texData.push_back(distance < 1.0f ? 255u : 0u);              // A
        }
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, texWidth, texSegmentHeight * texSegments);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texSegmentHeight * texSegments, GL_RGBA, GL_UNSIGNED_BYTE,
                    &texData[0]);
}

RenderableMesh::TriangleMode Tree::getTrianglesMode() {
    return GL_TRIANGLES;
}


// ====================================================================================================================
// ===================================== TREE LIBRARY (MAGIC NUMBER KINGDOM) ==========================================
// ====================================================================================================================


float blackTupeloRatio(float ratio) {
    return 0.5f + 0.5f * (1 - ratio);
}

const Tree::TreeParams Tree::blackTupelo() {
    TreeLevelParams level1(glm::radians(60.0f), glm::radians(140.0f), 50, 0.3f, 1.0f, 10, glm::radians(0.0f),
                           glm::radians(0.0f));
    level1.initVars(glm::radians(-40.0f), 0.0f, 0.05f, glm::radians(90.0f));

    TreeLevelParams level2(glm::radians(30.0f), glm::radians(140.0f), 25, 0.6f, 1.0f, 10, glm::radians(-10.0f),
                           glm::radians(0.0f));
    level2.initVars(glm::radians(10.0f), 0.0f, 0.1f, glm::radians(150.0f));

    TreeLevelParams level3(glm::radians(45.0f), glm::radians(140.0f), 12, 0.4f, 1.0f, 1, glm::radians(0.0f),
                           glm::radians(0.0f));
    level3.initVars(glm::radians(10.0f), 0.0f, 0.0f, glm::radians(0.0f));

    std::vector<const TreeLevelParams> levelParams;
    levelParams.push_back(level1);
    levelParams.push_back(level2);
    levelParams.push_back(level3);

    TreeParams params(levelParams, blackTupeloRatio);
    params.ratio = 0.015f;
    params.ratioPower = 1.2f;
    params.rootLength = 1.0f;
    params.rootCurveRes = 10;
    params.rootCurve = 0.0f;
    params.rootCurveBack = 0.0f;
    params.scale = 1.0f;
    params.rootTaper = 1.0f;
    params.baseSize = 0.4f;
    params.leafScale = 1.0f;
    params.leafForm = 0.3f;
    params.leafStem = 0.05f;

    return params;
}

float blackOakRatio(float offsetFactor) {
    return 0.2f + 0.8f * glm::sin(0.5f * glm::pi<float>() * (1 - offsetFactor));
}

const Tree::TreeParams Tree::blackOak() {
    TreeLevelParams level1(glm::radians(30.0f), glm::radians(80.0f), 40, 0.8f, 1.0f, 10, glm::radians(40.0f),
                           glm::radians(-70.0f));
    level1.initVars(glm::radians(-30.0f), 0.0f, 0.01f, glm::radians(150.0f));

    TreeLevelParams level2(glm::radians(45.0f), glm::radians(140.0f), 120, 0.2f, 1.0f, 3, glm::radians(0.0f),
                           glm::radians(-30.0f));
    level2.initVars(glm::radians(10.0f), 0.0f, 0.05f, glm::radians(-30.0f));

    TreeLevelParams level3(glm::radians(45.0f), glm::radians(140.0f), 0, 0.4f, 1.0f, 1, glm::radians(0.0f),
                           glm::radians(0.0f));
    level3.initVars(glm::radians(10.0f), 0.0f, 0.0f, glm::radians(0.0f));

    std::vector<const TreeLevelParams> levelParams;
    levelParams.push_back(level1);
    levelParams.push_back(level2);
    levelParams.push_back(level3);

    TreeParams params(levelParams, blackOakRatio);
    params.ratio = 0.015f;
    params.ratioPower = 1.2f;
    params.rootLength = 1.0f;
    params.rootCurveRes = 10;
    params.rootCurve = 0.0f;
    params.rootCurveBack = 0.0f;
    params.scale = 1.0f;
    params.rootTaper = 1.0f;
    params.baseSize = 0.4f;
    params.leafScale = .8f;
    params.leafForm = 0.3f;
    params.leafStem = 0.05f;

    return params;
}

const Tree::TreeParams Tree::testCone() {
    TreeParams params(std::vector<const TreeLevelParams>(), blackTupeloRatio);
    params.ratio = 0.015f;
    params.ratioPower = 1.2f;
    params.rootLength = 1.0f;
    params.rootCurveRes = 5;
    params.rootCurve = 1.40f;
    params.rootCurveBack = 0.0f;
    params.scale = 5.0f;
    params.rootTaper = 1.0f;
    params.baseSize = 1.0f;

    return params;
}


