//
// Created by botanick on 15.11.15.
//

#include "heightGenerator.h"
#include "noise.h"

std::atomic_int HEIGHT_CALLS(0);

float getHeight(float x, float y) {
    static const int noiseScale = 1 << 11;

    float result = noise(x / noiseScale, y / noiseScale);

    HEIGHT_CALLS++;
    return result * MAX_HEIGHT;
}

int getHeightCalls() {
    return HEIGHT_CALLS.load();
}

