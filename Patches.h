#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

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

  bool needReinit() const {
    return !vertices.empty();
  }
  
	bool needToDraw() const {
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
    running = false;

    offsetX = 0;
    offsetY = 0;
	}

	void init();
	void shutdown();

	PatchHolder acquire(const int &index);
  //threadsafe
  void updatePos(const glm::vec3 &updatePos);

private:
  typedef std::shared_ptr<std::mutex> pMutex;

  std::vector<pMutex> locks;
	std::vector<Patch> patches;

  int offsetX, offsetY;

  std::atomic<glm::vec3> cameraPosition;
  std::atomic<bool> running;
  
  std::shared_ptr<std::thread> backgndThread;

  void refreshThread();
	void refresh();
  void reinitPatch(const int &index, const int &x, const int &y, const int &lod);
  int countLevelOfDetail(const int &x, const int &y);
};
