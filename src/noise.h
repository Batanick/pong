#include <atomic>
static std::atomic_int CALL_COUNTER ;

static const float PERSISTENCE = 0.2f;
static const float AMPLITUDE = 1 / PERSISTENCE;
static const unsigned int OCTAVES = 8;

float noise(float x, float y);