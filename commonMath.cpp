#include <cstdlib>

bool isPower2(int x) {
	return ( (x > 0) && ((x & (x - 1)) == 0) );
}

int trim( int min, int max, int value) {
	if (value < min) {
		return min;
	} 

	return value > max ? max : value;
}

float getRandomFloat() {
	return (float)rand()/(float)RAND_MAX;
}