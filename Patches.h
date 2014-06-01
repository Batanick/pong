#include <vector>
#include <mutex>
#include <memory>

#include "glm.hpp"

#include "TerrainConstants.h"
#include "RenderCommon.h"

struct Patch {
	Patch() {
		x = -1;
		y = -1;
	}

	GLuint id;
	int lod;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	int x, y;

	bool needReinit() {
		return !vertices.empty();
	}

	bool needToDraw() {
		return lod >= 0;
	}
};

struct PatchHolder final {
	Patch &patch;
	std::unique_lock<std::mutex> lock;

	PatchHolder(Patch &patch, std::mutex &_lock ) :
		patch(patch), 
		lock(_lock) {
	}

	PatchHolder(PatchHolder&& other):
		patch(other.patch){
		lock = std::move(other.lock);
	}

	PatchHolder& operator=(PatchHolder&& that) {
		patch = Patch(that.patch);
		lock = std::move(that.lock);
		return *this;
	}

	PatchHolder(const PatchHolder &) = delete;
	PatchHolder& operator=(PatchHolder &) = delete;
};


class Patches final {
public:
	Patches() {
		//nothing here
	}

	void init();
	void shutdown();

	PatchHolder acquire(const int &index);

private:
	std::vector<Patch> patches;

	typedef std::shared_ptr<std::mutex> pMutex;
	std::vector<pMutex> locks;

	void reinitPatch(Patch &patch);
	void refresh(const RenderContext &context);
};
