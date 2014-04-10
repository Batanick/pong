#include "Bush.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtx\constants.hpp>
#include <gtx\transform.hpp>
#include <gtx\euler_angles.hpp>

#include "commonMath.h"
#include "renderUtils.h"

static char heightSegments = 4;

static float const leafPosScatering = 1.0f; 
static float const heightScatering = 0.3f;
static float const maxRotationAngle = glm::pi<float>() / 8;
static float const rotationAngleScatering = 0.3f;

void Bush::init( const GLuint shaderId ) {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    for ( int i = 0; i < leafsCount; i++ ) {
        const float yaw = glm::pi<float>() * 2 * getRandomFloat();
        const float height = (float)(this->height * ( 1 - heightScatering ) + this->height * heightScatering * getRandomFloat());
        const float rotationAngle = maxRotationAngle * ( 1 - rotationAngleScatering ) + maxRotationAngle * rotationAngleScatering * getRandomFloat();
        const glm::vec4 localPos = glm::eulerAngleY( glm::pi<float>() * 2 * getRandomFloat() ) * glm::vec4(leafPosScatering * getRandomFloat(), 0, 0, 0 );

        createLeaf(glm::vec3(localPos) + this->pos, height, rotationAngle, yaw, vertices, indices);
    }

    indicesSize = indices.size();
    
    // generating leaf buffers
    glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW );

    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );

    mvpId = glGetUniformLocation( shaderId, "mvp" );
}

void Bush::render( const RenderContext &context ) {
    glUniformMatrix4fv( mvpId, 1, GL_FALSE, &context.pv[0][0] );

    glEnableVertexAttribArray(0);
    
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements( GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void *) 0);

    glDisableVertexAttribArray(0);
}
 
void Bush::shutdown() {
    glDeleteBuffers ( 1, &indexBuffer );
    glDeleteBuffers ( 1, &vertexBuffer );
}

void Bush::createLeaf( glm::vec3 pos, float height, float maxRotationAngle, float localYaw, 
                      std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices) {
    const int startIndex = vertices.size();
    const glm::mat4 mainRotTranslation = glm::translate( pos ) * glm::eulerAngleY( localYaw );

    const float widthDelta = width / ( 2 * heightSegments);
    const float segmentHeight = height / ( heightSegments );

    const float rotationSegmentDelta = maxRotationAngle / heightSegments;
    for ( int col = 0; col < 2; col++ ) {
        for ( int row = 0; row < heightSegments; row++ ) {
            float const x = width / 2 + ((col % 2 == 0) ? 1 : -1)  * (heightSegments - row) * widthDelta;
            float const y = row * segmentHeight;

            glm::vec4 point (x, y, 0, 1);
            glm::vec4 rotated = mainRotTranslation * glm::eulerAngleX(row * rotationSegmentDelta) *  point;
            vertices.push_back( glm::vec3(rotated) ) ;
        }
    }
    vertices.push_back( glm::vec3( mainRotTranslation * glm::eulerAngleX(maxRotationAngle) * glm::vec4( width / 2, height, 0, 1) ));

    generateRowIndices( startIndex, heightSegments - 1,  indices );

    indices.push_back( heightSegments - 1 + startIndex );
    indices.push_back( heightSegments * 2 - 1 + startIndex );
    indices.push_back( heightSegments * 2 + startIndex );
}

