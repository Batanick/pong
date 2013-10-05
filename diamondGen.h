
class HeightMap final {
public:
	~HeightMap() {
		delete[] data;
	}

	HeightMap static create( const float h1, const float h2, const float h3, const float h4, int resolution );
	float getHeight( int x, int y ) const;
	float getMinHeight() const;
	float getMaxHeight() const;

private : 
	HeightMap( int resolution, float * const data, const float minValue, const float maxValue ) : 
		resolution( resolution ), data(data), minValue(minValue), maxValue(maxValue) {
	}

	float * const data;
	const int resolution;
	const float minValue;
	const float maxValue;
};
