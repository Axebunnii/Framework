#include <iostream>

#include <common/grid.h>

Grid::Grid(/*int xSize, int ySize, int cellSize*/) {
	/*rows = xSize;
	cols = ySize;

	cell = cellSize;

	_dynamic = false;

	drawGrid(rows, cols, cell*/
}

Grid::~Grid() {

}

void Grid::drawGrid(int xSize, int ySize, int cellSize) {
	for (int i = 1; i < xSize; i++) {
		line(0, 0, 0, i*cellSize);
		if (i >= xSize) {
			this->closed(true);
		}
	}
	for (int i = 0; i < ySize; i++) {
		line(0, 0, i*cellSize, 0);
		if (i >= ySize) {
			this->closed(true);
		}
	}
}