#include "noise.h"

#include <math.h>

static const int OCTAVES = 1;

float rand( int x, int y ) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

float smooth(float x, float y) {
    const float corners = ( rand(x-1, y-1) + rand(x+1, y-1) + rand(x-1, y+1) + rand(x+1, y+1) ) / 16;
    const float sides = ( rand(x-1, y) + rand(x+1, y) + rand(x, y-1) + rand(x, y+1) ) / 8;
    const float center = rand(x, y) / 4;
    return corners + sides + center;
}

float interpolate( float min, float max, float x ) {
    return min*(1 - x) + max*x;
}

float interpolatedNoise(float x, float y) {
    const int intX = static_cast<int>(x);
    const float fractX = x - intX;

    const int intY = static_cast<int>(y);
    const float fractY = y - intY;

    const float v1 = smooth( intX, intY );
    const float v2 = smooth( intX + 1, intY );
    const float v3 = smooth( intX, intY + 1);
    const float v4 = smooth( intX + 1, intY + 1 );

    const float i1 = interpolate(v1 , v2 , fractX);
    const float i2 = interpolate(v3 , v4 , fractX);

    return interpolate(i1 , i2 , fractY);
}


float noise(float x, float y) {
    float result = 0;
    const float persistence = 0.5f;
    
    for (int i = 0; i < OCTAVES; i++) {
        const float frequency = pow( 2.0f, i );
        const float amplitude = pow( persistence, i );
        result += interpolatedNoise(x * frequency, y * frequency) * amplitude;
    }

    return result;
}