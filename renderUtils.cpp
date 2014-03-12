#include "renderUtils.h"


void generateIndexTable( const int rows, const int cols, std::vector<unsigned int> &indices ) {
	int indidiesNeeded = rows * (2 * (cols + 1) - 1) + 1;  

	indices.push_back(0);

	int rowEndCounter = 2 * cols + 1;
	bool forward = true;
	bool right = true;
	int current = 0;
	for (int i = 1; i < indidiesNeeded ; i++){
		if (forward) {
			current += cols + 1;
		} else {
			current -= cols + 1;
			current += right ? 1 : -1;
		}

		indices.push_back(current);
		forward = !forward; 

		if ( (--rowEndCounter) == 0 ) {
			forward = true;
			right = !right;
			rowEndCounter = 2 * cols + 1;
		}
	}

}

void generateRowIndices( const int offset, const int cols, std::vector<unsigned int> &indices) {
    for (int i = offset; i < offset + cols; i++) {
        indices.push_back( i );
        indices.push_back( i + 1 );
        indices.push_back( i + cols + 1 );

        indices.push_back( i + 1 );
        indices.push_back( i + cols + 1);
        indices.push_back( i + cols + 2 );
    }
}