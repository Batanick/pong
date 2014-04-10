#include "Label.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

void Label::render( const RenderContext &context ) {
    if ( !loaded ) {
        initVertices( context );
    }

    glUniform3f( fontColorId, color.r, color.g, color.b );
    
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, font->getTextureInfo().textureId );
    glUniform1i( fontTextureId, 0 );

    glEnableVertexAttribArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0 );

    glEnableVertexAttribArray(1);
    glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * str.length() );

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

glm::mat3 convertToViewMatrix( float scaleX, float scaleY, int translateX, int translateY) {
    glm::mat3 proj;

    proj[0][0] = scaleX;
    proj[1][1] = scaleY;
    proj[0][2] = (float) translateX;
    proj[1][2] = (float) translateY;

    return proj; 
}

void Label::init( const GLuint shaderId ) {
    fontColorId = glGetUniformLocation( shaderId, "fontColor" );
    fontTextureId = glGetUniformLocation( shaderId, "texture" );

    glGenBuffers( 1, &vertexBuffer );
    glGenBuffers( 1, &uvBuffer );
}

void Label::shutdown() {
    if (loaded) {
        loaded = false;
        glDeleteBuffers (1, &vertexBuffer);
        glDeleteBuffers (1, &uvBuffer);
    }
}

void Label::initVertices( const RenderContext &context ) {
    //screen coords to view
    glm::mat3 const proj = convertToViewMatrix( 2 / (float) context.windowWidth, 2 / (float) context.windowHeight, -1, -1 );

    const TextureInfo info = font->getTextureInfo();
    glm::mat3 const uvProj = convertToViewMatrix( 1 / (float) info.width ,  1 / (float) info.height, 0, 0 );

    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> uvs;

    int currentX = x;
    for (unsigned int i = 0; i < str.length(); i++) {
        int const chr = str[i];
        Glyph const gl = font->getGlyph(chr);
        
        glm::vec2 p1(glm::vec3(currentX, y, 1) * proj);
        glm::vec2 p2(glm::vec3(currentX + gl.width, y, 1) * proj);
        glm::vec2 p3(glm::vec3(currentX + gl.width, y + gl.height, 1) * proj);
        glm::vec2 p4(glm::vec3(currentX, y + gl.height, 1) * proj);

        glm::vec2 uv1(glm::vec3(gl.x, gl.y + gl.height, 1) * uvProj);
        glm::vec2 uv2(glm::vec3(gl.x + gl.width, gl.y + gl.height, 1) * uvProj);
        glm::vec2 uv3(glm::vec3(gl.x + gl.width, gl.y, 1) * uvProj);
        glm::vec2 uv4(glm::vec3(gl.x, gl.y, 1) * uvProj);
        
        vertices.push_back( p1 );
        uvs.push_back( uv1 );
        vertices.push_back( p2 );
        uvs.push_back( uv2 );
        vertices.push_back( p4 );
        uvs.push_back(uv4);
        vertices.push_back( p2 );
        uvs.push_back(uv2);
        vertices.push_back( p3 );
        uvs.push_back(uv3);
        vertices.push_back( p4 );
        uvs.push_back(uv4);

        currentX += gl.width + font->getDistance();
    }

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
    glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_DYNAMIC_DRAW );

    loaded = true;
}

void Label::setText(std::string text) {
    if (text == str) {
        return;
    }

    shutdown();
    str = text;
}