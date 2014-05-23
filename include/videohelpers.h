#include <stdint.h>

class Texture {
};

class RectangularGrid {
	public:
	RectangularGrid(int resolution);
	~RectangularGrid();
	void resetGrid();
	void render();

	private:
	int resolution;
	uint32_t *indices; // stride: 6 items
	float *vertices; // stride: 5 items: xyzuv
};
