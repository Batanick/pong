#include "Patches.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "logging.h"
#include "noise.h"

int countLevelOfDetail(const int &x, const int &y);
float getHeight(float x, float y);
void generateVertices(const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod);

void Patches::init() {
	patches.resize(PATCHES_COUNT);
	
	std::vector<GLuint> vBuffers(PATCHES_COUNT);
	glGenBuffers(PATCHES_COUNT, &vBuffers[0]);

	for (int i = 0; i < PATCHES_COUNT; i++) {
		pMutex lock = std::shared_ptr<std::mutex>( new std::mutex() );
		locks.push_back( lock );

		Patch &patch = patches[i];
		patch.id = vBuffers[i];

		const int row = i / PATCHES_COUNT_SQRT;
		const int column = i % PATCHES_COUNT_SQRT;
		
		patch.lod = countLevelOfDetail(column, row);
		patch.x = column;
		patch.y = row;

		glBindBuffer(GL_ARRAY_BUFFER, patch.id);

		const long dataSize = VERTICES_IN_PATH * sizeof(glm::vec3);
		glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW);

		PatchHolder holder = acquire(i);
		reinitPatch( patch );
	}
}

void Patches::refresh(const RenderContext &context) {
	const glm::vec3 cameraPos = context.cameraPos;

	const int x = static_cast<int>(round(cameraPos.x / PATCH_SIZE_METERS));
	const int y = static_cast<int>(round(cameraPos.z / PATCH_SIZE_METERS));

	for (int i = 0; i < PATCHES_COUNT; i++) {
		reinitPatch(patches[i]);
	}
}

void Patches::reinitPatch(Patch &patch) {
	const glm::vec2 offset(patch.x * PATCH_SIZE_METERS + TERRAIN_OFFSET, patch.y * PATCH_SIZE_METERS + TERRAIN_OFFSET);
	generateVertices( offset, patch.vertices, patch.lod);

	const double start = glfwGetTime();

	glBindBuffer(GL_ARRAY_BUFFER, patch.id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, patch.vertices.size() * sizeof(glm::vec3), &patch.vertices[0]);
	glFinish();

	if (patch.lod == 0) {
		LOG("Buffering [%d]: %dms", patch.lod, static_cast<int>((glfwGetTime() - start) * 1000));
	}
}

PatchHolder Patches::acquire(const int &index) {
	Patch& patch = patches[index];
	std::mutex &lock = *locks[index];

	PatchHolder holder = PatchHolder( patch, lock );
	return holder;
}

void Patches::shutdown() {
	for (const auto &patch : patches) {
		glDeleteBuffers(1, &patch.id);
	}
}

int countLevelOfDetail(const int &x, const int &y) {
	static const float indexOffset = PATCHES_COUNT_SQRT / 2 - 0.5;
	const float floatLod = glm::max(glm::abs(static_cast<float>(x)-indexOffset), glm::abs(static_cast<float>(y)-indexOffset)) / LOD_STEP;
	const int lod = glm::min(glm::max(0, static_cast<int>(floatLod)), LOD_LEVELS_COUNT - 1);
	return lod;
}

void generateVertices(const glm::vec2 offset, std::vector<glm::vec3> &vertices, int lod) {
	const double start = glfwGetTime();

	const int factor = glm::min(TILES_IN_PATCH_SQRT, 1 << lod * LOD_REDUCTION);
	const float tileSize = TILE_SIZE * factor;
	const int tilesCount = TILES_IN_PATCH_SQRT / factor;

	for (int y = 0; y < tilesCount + 1; y++) {
		for (int x = 0; x < tilesCount + 1; x++) {
			const float xCoord = x * tileSize + offset.x;
			const float yCoord = y * tileSize + offset.y;
			const float zCoord = getHeight(xCoord, yCoord); // * MAX_HEIGHT;
			vertices.push_back(glm::vec3(xCoord, zCoord, yCoord));
		}
	}

	if (lod == 0) {
		LOG("Reiniting lod[%d]: %dms", lod, static_cast<int>((glfwGetTime() - start) * 1000));
	}
}

float getHeight(float x, float y) {
	float result = glm::max(noise(x / 64, y / 64) + 0.8f, 0.0f) / 2.0f;
	return result * MAX_HEIGHT;
}