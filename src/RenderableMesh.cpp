#include "RenderableMesh.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void RenderableMesh::init(const GLuint shaderId) {
    MeshContext mesh;
    initMesh(mesh);

    indicesSize = mesh.indices.size();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(TexVertexData), &(mesh.vertices[0]), GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &(mesh.indices[0]),
                 GL_STATIC_DRAW);

    initTexture(textureId);

    worldId = glGetUniformLocation(shaderId, "world");
    projectionId = glGetUniformLocation(shaderId, "projection");
    viewId = glGetUniformLocation(shaderId, "view");
    lightDirId = glGetUniformLocation(shaderId, "lightDir");
    textureParamId = glGetUniformLocation(shaderId, "texture");
}

void RenderableMesh::render(const RenderContext &context) {
    glUniformMatrix4fv(projectionId, 1, GL_FALSE, &context.projection[0][0]);
    glUniformMatrix4fv(viewId, 1, GL_FALSE, &context.view[0][0]);
    glUniformMatrix4fv(worldId, 1, GL_FALSE, &getWorld()[0][0]);
    glUniform3fv(lightDirId, 1, &context.lightDir[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureParamId, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    static const int coordsOffset = offsetof(TexVertexData, position);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *) coordsOffset);

    static const int normalOffset = offsetof(TexVertexData, normal);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *) normalOffset);

    static const int uvOffset = offsetof(TexVertexData, uv);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexVertexData), (void *) uvOffset);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(getTrianglesMode(), indicesSize, GL_UNSIGNED_INT, (void *) 0);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void RenderableMesh::shutdown() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

glm::mat4 RenderableMesh::getWorld() {
    return glm::mat4();
}

RenderableMesh::TriangleMode RenderableMesh::getTrianglesMode() {
    return GL_TRIANGLE_STRIP;
}

