#include "Clouds.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

void Clouds::init(const GLuint shaderId) {
    pvId = glGetUniformLocation(shaderId, "vp");
    cameraPosId = glGetUniformLocation(shaderId, "cameraPos");
    timeId = glGetUniformLocation(shaderId, "time");

    std::vector<glm::vec3> vertices;
    for (int i = SKY_LEVELS_COUNT; i > 0; i--) {
        const float height = HEIGHT + i * HEIGHT_STEP;

        vertices.push_back(glm::vec3(SIZE, height, SIZE));
        vertices.push_back(glm::vec3(-SIZE, height, SIZE));
        vertices.push_back(glm::vec3(-SIZE, height, -SIZE));

        vertices.push_back(glm::vec3(-SIZE, height, -SIZE));
        vertices.push_back(glm::vec3(SIZE, height, -SIZE));
        vertices.push_back(glm::vec3(SIZE, height, SIZE));
    }

    glGenBuffers(1, &verticesId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}

void Clouds::render(const RenderContext &context) {
    glUniformMatrix4fv(pvId, 1, GL_FALSE, &context.pv[0][0]);
    glUniform3fv(cameraPosId, 1, &context.cameraPos[0]);

    const float time = (float) context.time;
    glUniform1f(timeId, time);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glDrawArrays(GL_TRIANGLES, 0, 6 * SKY_LEVELS_COUNT);

    glDisableVertexAttribArray(0);
}

void Clouds::shutdown() {
    glDeleteBuffers(1, &verticesId);
}

ShaderType Clouds::getType() {
    return ShaderType::CLOUDS_SHADER;
}