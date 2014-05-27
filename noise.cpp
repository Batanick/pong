#include "noise.h"

#include <math.h>
#include "commonMath.h"

static const int OCTAVES = 8;

inline float signum(float n) {
    if (n < 0) 
        return -1;
    if (n > 0) 
        return 1;
    return 0;
}

float rand( int x, int y ) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return static_cast<float>( 1.0 - ((double)nn / 1073741824.0) );
}

float smooth(int x, int y) {
    const float corners = ( rand(x-1, y-1) + rand(x+1, y-1) + rand(x-1, y+1) + rand(x+1, y+1) ) / 16;
    const float sides = ( rand(x-1, y) + rand(x+1, y) + rand(x, y-1) + rand(x, y+1) ) / 8;
    const float center = rand(x, y) / 4;
    return corners + sides + center;
}

float interpolate( float min, float max, float x ) {
    x *= glm::pi<float>();
    x = (1 - glm::cos<float>(x)) * 0.5f;
    return min*(1 - x) + max*x;
}

float interpolatedNoise(float x, float y) {
    const int intX1 = static_cast<int>(x);
    const int intX2 = intX1 + static_cast<int>( signum(x) );
    const float fractX = abs(x - intX1);

    const int intY1 = static_cast<int>(y);
    const int intY2 = intY1 + static_cast<int>( signum(y) );
    const float fractY = abs(y - intY1);

    const float v1 = rand( intX1, intY1 );
    const float v2 = rand( intX2, intY1 );
    const float v3 = rand( intX1, intY2);
    const float v4 = rand( intX2, intY2 );

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