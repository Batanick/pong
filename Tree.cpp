#include "Tree.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx\constants.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>

#include "commonMath.h"

void Tree::init() {
    std::vector<const glm::vec3> vertices;
    std::vector<unsigned int> indices;

    const TreeParams treeParams = getParams();
    StemParams rootParams;
    rootParams.direction = glm::normalize( glm::vec3(0, 1, 0) );
    rootParams.curveAxis = glm::normalize( glm::vec3(0, 0, 1) );
    rootParams.length = treeParams.rootLength * treeParams.scale;
    rootParams.radius = rootParams.length * treeParams.scale * treeParams.ratio;
    rootParams.resolution = 16;
    rootParams.segments = treeParams.rootCurveRes;
    rootParams.curve = treeParams.rootCurve;
    rootParams.weist = 1 - treeParams.rootTaper;
    rootParams.pos = glm::vec3(0,0,0);

    drawStem( rootParams, vertices, indices );

    const TreeLevelParams levelParams = treeParams.getParams(1);
    const float baseSize = treeParams.baseSize * rootParams.length;
    const int branchesCount = (int)glm::round(levelParams.branches * (0.2f + 0.8f * ( levelParams.length / rootParams.length  )));
    const float branchesDistance = (rootParams.length * ( 1 - treeParams.baseSize) ) / branchesCount;
    const float downAngle = levelParams.downAngle;
    const float offsetLength = rootParams.length - baseSize;

    float rotation = 0;
    for (int i = 0; i < branchesCount; i++) {
        const float offsetFactor = ((float) i) / (branchesCount + 1);
        const float offset = offsetLength * offsetFactor;

        StemParams childParams;
        childParams.curve = levelParams.curve;

        const glm::mat3 rotationMat = glm::toMat3( glm::angleAxis( glm::degrees(rotation), rootParams.direction) * glm::angleAxis( glm::degrees( downAngle ) , rootParams.curveAxis )  );
        childParams.direction = glm::normalize( rotationMat * rootParams.direction);

        childParams.curveAxis = glm::normalize(glm::cross(childParams.direction, rootParams.direction));

        const float shapeRatio = 0.2f + 0.8f * glm::sin(0.5f * glm::pi<float>() * (1 - offsetFactor));
        childParams.length = levelParams.length * rootParams.length * shapeRatio;
        childParams.pos = rootParams.direction * ( baseSize + offset );
        childParams.radius = rootParams.radius * glm::pow( childParams.length / rootParams.length, treeParams.ratioPower );
        childParams.resolution = rootParams.resolution;
        childParams.segments = levelParams.curveRes;
        childParams.weist = 1- levelParams.taper;

        drawStem(childParams, vertices, indices);
        rotation += levelParams.rotate;
    }
    

    indicesCount = indices.size();
    // generating leaf buffers
    glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW );

    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );
}


void Tree::drawStem( const StemParams &stem, std::vector<const glm::vec3> &vertices, std::vector<unsigned int> &indices ) {
    const int offset = vertices.size();
    const float segmentHeight = stem.length / stem.segments;
    const float yawDelta = glm::pi<float>() * 2 / stem.resolution;
    const float radiusWaistFactor = ( 1 - stem.weist ) / stem.segments;

    const float curveAngle = stem.curve / (stem.segments);

    // ================== VERTICES ================
    float yaw = 0;
    glm::vec3 pos = stem.pos;
    glm::vec3 increment = glm::normalize(stem.direction) * segmentHeight;
    for ( int row = 0; row <= stem.segments; row++) {
        glm::vec3 rotor = stem.curveAxis * stem.radius * (1 - radiusWaistFactor * row );
        
        for ( int col = 0; col <= stem.resolution; col++ ) {

            const glm::vec3 current = pos + glm::vec3( glm::angleAxis( glm::degrees(yaw), glm::normalize(increment) ) * rotor );
            vertices.push_back( current );
            yaw += yawDelta;
        }
        ;
        increment = glm::angleAxis( glm::degrees(curveAngle) ,glm::normalize(stem.curveAxis) ) * increment;
        pos += increment;
    }

    // ================== INDICES ================
    for ( int row = 0; row < stem.segments; row++) {
        for ( int col = 0; col < stem.resolution; col++ ) {
            indices.push_back( col + row * (stem.resolution + 1) + offset );
            indices.push_back( col + row * (stem.resolution + 1) + 1 + offset);
            indices.push_back( col + (row + 1) * (stem.resolution + 1) + offset );
            
            indices.push_back( col + row * (stem.resolution + 1) + 1 + offset);
            indices.push_back( col + (row + 1) * (stem.resolution + 1) + 1 + offset);
            indices.push_back( col + (row + 1) * (stem.resolution + 1) + offset);
        }
    }

}

void Tree::render( const RenderContext &context ) {
    glUniformMatrix4fv( context.bushMVPId, 1, GL_FALSE, &context.pv[0][0] );

    glEnableVertexAttribArray(0);
    
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements( GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void *) 0);

    glDisableVertexAttribArray(0);
}
 
void Tree::shutdown() {
    glDeleteBuffers ( 1, &indexBuffer );
    glDeleteBuffers ( 1, &vertexBuffer );
}

const Tree::TreeParams Tree::getParams() {
    std::vector<const TreeLevelParams> levelParams;
    levelParams.push_back( TreeLevelParams( glm::radians(-30.0f), glm::radians(80.0f), 40, 0.8f, 1.0f, 10, glm::radians(40.0f), glm::radians(-70.0f)) );
    levelParams.push_back( TreeLevelParams( glm::radians(45.0f), glm::radians(140.0f), 120, 0.2f, 1.0f, 3, glm::radians(0.0f), glm::radians(-30.0f)) );
    levelParams.push_back( TreeLevelParams( glm::radians(45.0f), glm::radians(140.0f), 0, 0.4f, 1.0f, 1, glm::radians(0.0f), glm::radians(0.0f)) );

    TreeParams params(levelParams);
    params.ratio = 0.018f;
    params.ratioPower = 1.3f;
    params.rootLength = 1.0f;
    params.rootCurveRes = 8;
    params.rootCurve = 0.0f;
    params.rootCurveBack = 0;
    params.scale = 1.0f;
    params.rootTaper = 0.95f;
    params.baseSize = 0.25f;

    return params;
}

 