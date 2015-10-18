#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <shared_mutex>

#include "glm/glm.hpp"

#include "TerrainConstants.h"
#include "RenderCommon.h"

int getHeightCalls();

struct Patch {
    Patch() {
        x = -1;
        y = -1;
    }

    GLuint id;
    int lod;
    std::vector<VertexData> vertices;
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

    PatchHolder(Patch &patch, std::mutex &_lock) :
            patch(patch),
            lock(_lock) {
    }

    PatchHolder(PatchHolder &&other) :
            patch(other.patch) {
        lock = std::move(other.lock);
    }

    PatchHolder &operator=(PatchHolder &&that) {
        patch = Patch(that.patch);
        lock = std::move(that.lock);
        return *this;
    }

    PatchHolder(const PatchHolder &) = delete;

    PatchHolder &operator=(PatchHolder &) = delete;
};


class Patches final {
public:
    Patches() {
        cameraPosition = glm::vec3();
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

    int offsetX = 0;
    int offsetY = 0;

    std::shared_timed_mutex rwLock;
    glm::vec3 cameraPosition;

    volatile bool running;

    std::shared_ptr<std::thread> backgndThread;

    void refreshThread();

    void refresh();

    void reinitPatch(const int &index, const int &x, const int &y, const int &lod);

    int countLevelOfDetail(const int &x, const int &y);
};
