
#include "Terrain.h"

#include <GLFW/glfw3.h>

#include "logging.h"
#include "renderUtils.h"

//#define STATIC_TERRAIN

void Terrain::init(const GLuint shaderId) {
    projectionId = glGetUniformLocation(shaderId, "projection");
    viewId = glGetUniformLocation(shaderId, "view");
    textureParamId = glGetUniformLocation(shaderId, "terrainTexture");
    heightId = glGetUniformLocation(shaderId, "heightScale");
    cameraPosId = glGetUniformLocation(shaderId, "cameraPos");
    lightDirId = glGetUniformLocation(shaderId, "lightDir");
    waterLevel = glGetUniformLocation(shaderId, "waterLevel");

    initIndices(shaderId);
    initVertices(shaderId);
    initTexture(shaderId);
}

void Terrain::initIndices(const GLuint &shaderId) {
    indexBuffers.resize(LOD_LEVELS_COUNT);
    std::vector<GLuint> iBuffers(LOD_LEVELS_COUNT);
    glGenBuffers(LOD_LEVELS_COUNT, &iBuffers[0]);

    int size = TILES_IN_PATCH_SQRT;
    for (int i = 0; i < LOD_LEVELS_COUNT; i++) {
        std::vector<unsigned int> indices;

        generateIndexTable(size + 1, size + 1, indices);
        LOG("Lod %d: [%d] elements", i, size);
        size = size >> 1;

        indexBuffers[i].id = iBuffers[i];
        indexBuffers[i].length = indices.size();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].length * sizeof(unsigned int), &indices[0],
                     GL_STATIC_DRAW);
    }
}

void Terrain::initVertices(const GLuint &shaderId) {
    patches->init();
}

void Terrain::initTexture(const GLuint &shaderId) {
    const int textureLength = 256;
    unsigned char *textureData = new unsigned char[textureLength * 4];

    const std::vector<std::pair<float, color>> colors = buildTerrainColors();

    unsigned int n = 0;
    for (int i = 0; i < textureLength; i++) {
        const float u = ((float) i) / textureLength;

        if ((colors[n].first < u) && (n < (colors.size() - 1))) {
            n++;
        }

        const color clr = colors[n].second;
        textureData[i * 4] = clr.r;                    // R
        textureData[i * 4 + 1] = clr.g;              // G
        textureData[i * 4 + 2] = clr.b;              // B
        textureData[i * 4 + 3] = clr.maxDisloc; //max value of U coord dislocation
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_1D, textureId);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA8, textureLength);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, textureLength, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    delete[] textureData;
}

void Terrain::render(const RenderContext &context) {
#ifndef STATIC_TERRAIN
    patches->updatePos(context.cameraPos);
#endif

    glUniformMatrix4fv(projectionId, 1, GL_FALSE, &context.projection[0][0]);
    glUniformMatrix4fv(viewId, 1, GL_FALSE, &context.view[0][0]);
    glUniform1f(heightId, MAX_HEIGHT);
    glUniform3fv(cameraPosId, 1, &context.cameraPos[0]);
    glUniform3fv(lightDirId, 1, &context.lightDir[0]);
    glUniform4f(waterLevel, 0, 1, 0, 0);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, textureId);
    glUniform1i(textureParamId, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    for (int i = 0; i < PATCHES_COUNT; i++) {
        PatchHolder holder = patches->acquire(i);
        Patch &patch = holder.patch;
        if (!patch.needToDraw()) {
            continue;
        }
        refreshPatch(patch);

        glBindBuffer(GL_ARRAY_BUFFER, patch.id);

        static const int coordsOffset = offsetof(VertexData, position);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) coordsOffset);

        static const int normalOffset = offsetof(VertexData, normal);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) normalOffset);

        const IndexBuffer &indexBuffer = indexBuffers[patch.lod];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) indexBuffer.length, GL_UNSIGNED_INT, (void *) 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Terrain::refreshPatch(Patch &patch) {
    if (!patch.needReinit()) {
        return;
    }

    const double start = glfwGetTime();

    glBindBuffer(GL_ARRAY_BUFFER, patch.id);
    glBufferData(GL_ARRAY_BUFFER, patch.vertices.size() * sizeof(VertexData), &patch.vertices[0], GL_DYNAMIC_DRAW);
    glFinish();

    patch.vertices.clear();

    if (patch.lod == 0) {
        LOG("Refreshing lod[%d]: %dms", patch.lod, static_cast<int>((glfwGetTime() - start) * 1000));
    }
}

void Terrain::shutdown() {
    for (const auto buffer : indexBuffers) {
        glDeleteBuffers(1, &buffer.id);
    }

    patches->shutdown();
    glDeleteTextures(1, &textureId);
}

ShaderType Terrain::getType() {
    return ShaderType::TERRAIN_SHADER;
}