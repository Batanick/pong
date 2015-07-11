#include <memory>

class Tickable {
public:
    virtual void tick(long delta) = 0;
};

typedef std::weak_ptr<Tickable> TickablePtr;