#include "Bush.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtx\constants.hpp>
#include <gtx\transform.hpp>
#include <gtx\euler_angles.hpp>

#include "commonMath.h"
#include "renderUtils.h"

static char heightSegments = 16;

static float const leafPosScatering = 1.0f; 
static float const heightScatering = 0.3f;
static float const maxRotationAngle = glm::pi<float>() / 8;
static float const rotationAngleScatering = 0.3f;

void Bush::init() {
    for ( int i = 0; i < leafsCount; i++ ) {
        const float yaw = glm::pi<float>() * 2 * getRandomFloat();
        const float height = (float)(this->height * ( 1 - heightScatering ) + this->height * heightScatering * getRandomFloat());
        const float rotationAngle = maxRotationAngle * ( 1 - rotationAngleScatering ) + maxRotationAngle * rotationAngleScatering * getRandomFloat();
        const glm::vec4 localPos = glm::eulerAngleY( glm::pi<float>() * 2 * getRandomFloat() ) * glm::vec4(leafPosScatering * getRandomFloat(),0,0, 0);

        leafs.push_back( createLeaf(glm::vec3(localPos) + this->pos, height, rotationAngle, yaw) );
    }
}

void Bush::render( const RenderContext &context ) {
    glUniformMatrix4fv( context.bushMVPId, 1, GL_FALSE, &context.pv[0][0] );

    for (unsigned int i = 0; i < leafs.size(); i++) {
        Leaf &leaf = leafs[i];

        glEnableVertexAttribArray(0);

        glBindBuffer( GL_ARRAY_BUFFER, leaf.vertexBuffer );
    	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, leaf.indexBuffer);
        glDrawElements( GL_TRIANGLE_STRIP, leaf.indexSize, GL_UNSIGNED_INT, (void *) 0);

	    glDisableVertexAttribArray(0);
    }
}
 
void Bush::shutdown() {
    for (unsigned int i = 0; i < leafs.size(); i++) {
        Leaf &leaf = leafs[i];
        glDeleteBuffers ( 1, &leaf.indexBuffer );
        glDeleteBuffers ( 1, &leaf.vertexBuffer );
    }

    leafs.clear();
}

Bush::Leaf Bush::createLeaf( glm::vec3 pos, float height, float maxRotationAngle, float localYaw ) {
    glm::mat4 mainRotTranslation = glm::translate( pos ) * glm::eulerAngleY( localYaw );

    Bush::Leaf leaf;

    std::vector<glm::vec3> verticies;

    const float widthDelta = width / ( 2 * heightSegments);
    const float segmentHeight = height / ( heightSegments );

    const float rotationSegmentDelta = maxRotationAngle / heightSegments;
    for ( int col = 0; col < 2; col++ ) {
        for ( int row = 0; row < heightSegments; row++ ) {
            float const x = width / 2 + ((col % 2 == 0) ? 1 : -1)  * (heightSegments - row) * widthDelta;
            float const y = row * segmentHeight;

            glm::vec4 point (x, y, 0, 1);
            glm::vec4 rotated = mainRotTranslation * glm::eulerAngleX(row * rotationSegmentDelta) *  point;
            verticies.push_back( glm::vec3(rotated) ) ;
        }
    }

    verticies.push_back( glm::vec3( mainRotTranslation * glm::eulerAngleX(maxRotationAngle) * glm::vec4( width / 2, height, 0, 1) ));

    std::vector<unsigned int> indices;
    generateIndexTable( 1, heightSegments - 1, indices );
    indices.push_back(verticies.size() - 1);
    leaf.indexSize = indices.size();

    // generating leaf buffers
    glGenBuffers( 1, &leaf.vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, leaf.vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, verticies.size() * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW );

    glGenBuffers( 1, &leaf.indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, leaf.indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW );

    return leaf;
}

