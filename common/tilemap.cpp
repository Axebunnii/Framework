#include <iostream>

#include "tilemap.h"

TileMap::TileMap(const std::string& filename, int col, int row, float tsize) {
	cols = 0;
	rows = 0;
	tileSize = 0;
}

TileMap::~TileMap() {

}