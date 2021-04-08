#include <iostream>

#include "tilemap.h"

TileMap::TileMap() {
	/*tsize = tileSize;
	std::cout << "filename: " << filename << std::endl;
	f = filename;*/
}

TileMap::~TileMap() {

}

void TileMap::DrawMap(const std::string& filename, char* map, int rows, int cols, float tileSize) {
	r = rows;
	c = cols;
	//tsize = tileSize;
	std::cout << "filename: " << filename << std::endl;

	size_t size = r * c;
	for (size_t i = 0; i < size; i++) {
		//this->maplist.push_back(map[i]);
		Sprite* tile = new Sprite(filename);
		maplist.push_back(tile);
		std::cout << maplist.size() << std::endl;
	}

	for (int i = 0; i < r; i++) {
		for (int ii = 0; ii < c; ii++) {
			Sprite* tile = GetTile(c, r);
			tile->position.x = i * tsize;
			tile->position.y = ii * tsize;
			//float x = c * tsize;
			//float y = r * tsize;
			//drawTile(tile, x, y);
		}
	}
}

Sprite* TileMap::GetTile(int x, int y) {
	return 0;
}