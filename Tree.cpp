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

    StemParams params;
    params.direction = glm::normalize(glm::vec3(0, 1, 0));
    params.length = 1;
    params.radius = 0.005f;
    params.resolution = 16;
    params.segments = 128;
    params.curve = glm::pi<float>();
    params.weist = 1.0f;
    params.baseSize = 0.4f;
    params.pos = glm::vec3(0,0,0);
    params.maxBranches = 3;

    drawStem( params, vertices, indices );

    indicesCount = indices.size();
    // generating leaf buffers
    glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW );

    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );
}


void Tree::drawStem( const StemParams stem, std::vector<const glm::vec3> &vertices, std::vector<unsigned int> &indices ) {
    const glm::quat rotation = getRotation( glm::vec3(0, 1, 0), stem.direction );
    const glm::vec3 axis = glm::axis( rotation );
    const glm::mat4 translation = glm::translate(glm::mat4(), stem.pos );
    
    const int offset = vertices.size();
    const float segmentWidth = 2 * stem.radius * glm::sin(glm::radians(glm::pi<float>() ));
    const float segmentHeight = stem.length / stem.segments;

    const float yawDelta = glm::pi<float>() * 2 / stem.resolution;
    const float radiusWaistFactor = ( 1 - stem.weist ) / stem.segments;

    const float curveAngle = stem.curve / (stem.segments * 2);

    // ================== VERTICES ================
    float yaw = 0;
    for ( int row = 0; row <= stem.segments; row++) {
        for ( int col = 0; col <= stem.resolution; col++ ) {                            // Building a cylinder here.
            
            const glm::vec4 value = glm::eulerAngleY( yaw )                             // rotation around X axis
                * glm::vec4(stem.radius * (1 - radiusWaistFactor * row ), 0, 0, 1 )     // with schrinking radius
                + glm::vec4( 0, row * segmentHeight, 0, 0 );                            // adding height here

            const glm::quat curveTransform = glm::angleAxis( glm::degrees(curveAngle) * row , axis );

            vertices.push_back( glm::vec3( translation * glm::toMat4( rotation * curveTransform ) * value ));
            yaw += yawDelta;
        }
    }

    // ================== INDICES ================
    for ( int row = 0; row < stem.segments; row++) {
        for ( int col = 0; col < stem.resolution; col++ ) {
            indices.push_back( col + row * (stem.resolution + 1) );
            indices.push_back( col + row * (stem.resolution + 1) + 1);
            indices.push_back( col + (row + 1) * (stem.resolution + 1) );
            
            indices.push_back( col + row * (stem.resolution + 1) + 1);
            indices.push_back( col + (row + 1) * (stem.resolution + 1) + 1 );
            indices.push_back( col + (row + 1) * (stem.resolution + 1) );
        }
    }

   /*
   static bool first (true);
   if (!first) 
        return;

    first = false;
    
    float ratio = 0.015f;
    float ratioPower = 1.2f;

    StemParams params;
    params.length = (float) (stem.length * ( 0.2f + 0.8f * ratio) * 0.5);
    params.direction = stem.direction * glm::angleAxis ( glm::degrees(60.0f), axis );
    
    params.radius = stem.radius * (params.length / stem.length);
    params.resolution = stem.resolution;
    params.segments = stem.segments;
    params.curve = glm::pi<float>() / 18;
    params.weist = 0.0f;
    params.baseSize = 0.4f;


    params.pos = glm::vec3(0,0,0);
    params.maxBranches = 3;


    drawStem( params, vertices, indices );*/

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

 