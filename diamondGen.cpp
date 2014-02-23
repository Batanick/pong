#include "diamondGen.h"

#include "logging.h"
#include "commonMath.h"

static const int NOT_ASSIGNED = -1;
static const float ROUGHNESS  = 0.005f;

float assignValueIfNeeded( float * val1, float * val2, int size ) {
	if ( ((val2 - val2) % 2) != 0 ) {
		return 0;
	}

	const int targetOffset =  ( (val2 - val1) / (2));
	float * const targetValue = val1 + targetOffset;
	if (*targetValue == NOT_ASSIGNED) {
		*targetValue = (*val2 + *val1) / 2 + ( ROUGHNESS  * ( 2 * getRandomFloat() - 1 ) * size );
	}

	return *targetValue;
}

void build ( float * const data, int x1, int y1, int x2, int y2, int res ){
	const int size = x2 - x1;

	float * const topLeft = &data[ x1 + y1*res ];
	float * const topRight = &data[ x2 + y1*res ];
	float * const botRight = &data[ x2 + y2*res ];
	float * const botLeft = &data[ x1 + y2*res ];

	const int diamondX = ( x2 - x1 ) / 2 + x1;
	const int diamondY = ( y2 - y1 ) / 2 + y1;

	float diamondSumm = 0;

	diamondSumm += assignValueIfNeeded( topLeft, botLeft, size );
	diamondSumm += assignValueIfNeeded( topRight, botRight, size );
	diamondSumm += assignValueIfNeeded( botLeft, botRight, size );
	diamondSumm += assignValueIfNeeded( topLeft, topRight, size );

	data[ diamondX + diamondY * res ] = diamondSumm / 4;

	if ( (x2 - x1) <= 2 || (y2 - y1) <= 2) {
		return;
	}

	build( data, x1, y1, diamondX, diamondY, res ); 
	build( data, diamondX, y1, x2, diamondY, res ); 
	build( data, x1, diamondY, diamondX, y2, res ); 
	build( data, diamondX, diamondY, x2, y2, res ); 
}

HeightMap HeightMap::create( const float h1, const float h2, const float h3, const float h4, int resolution ) {
	VERIFY( isPower2( resolution - 1 ), "Resoulution + 1 must be power of 2", return HeightMap( resolution, 0, 0.0, 0.0));

	float * const data = new float[resolution * resolution] ;
	for (int i = 0; i < resolution * resolution; i++) {
		data[i] = NOT_ASSIGNED;
	}

	data[0] = h1;
	data[resolution - 1] = h2;
	data[(resolution - 1) * resolution] = h3;
	data[resolution * resolution - 1] = h4;

	build( data, 0, 0, resolution - 1, resolution - 1, resolution );

	float min = data[0];
	float max = data[0];
	for ( int i = 1; i < resolution * resolution; i++  ) {
		 min = data[i] < min ? data[i] : min;
		 max = data[i] > max ? data[i] : max;
	}

	return HeightMap( resolution, data, min, max );
}

float HeightMap::getHeight( int x, int y ) const {
	return( data[trim(0, resolution - 1, x) + resolution * trim(0, resolution - 1, y)] );
}

float HeightMap::getMaxHeight() const {
	return maxValue;
}

float HeightMap::getMinHeight() const {
	return minValue;
}