#include <iostream>

#include "tilemap.h"

TileMap::TileMap() {
	
}

TileMap::~TileMap() {

}

void TileMap::DrawMap(const std::string& filename, char* map, int rows, int cols, float tileSize) {
	r = rows;
	c = cols;
	std::cout << "filename: " << filename << std::endl;
	int t;

	size_t size = r * c;
	for (size_t i = 0; i < size; i++) {
		t = GetTile(filename, tileSize, map[i]);
		std::cout << "tile: " << t << std::endl;
	}
		
	for (int ix = 0; ix < r; ix++) {
		for (int iy = 0; iy < c; iy++) {
			Sprite* tile = new Sprite(filename);

			tile->position.x = ix * tileSize;
			tile->position.y = iy * tileSize;

			std::cout << "x: " << tile->position.x << std::endl;
			std::cout << "y: " << tile->position.y << std::endl;

			maplist.push_back(tile);
			std::cout << maplist.size() << std::endl;
		}
	}
}

int TileMap::GetTile(const std::string& filename, float tileSize, int f) {
	float uvwidth = 1.0f / tileSize;
	float uvheight = 1.0f / tileSize;

	//the part of the spritesheet that will be rendered
	glm::vec3 uvoffset = glm::vec3(0.0f, 0.0f, 0.0f);

	int w = 1.0f / uvwidth;
	int h = 1.0f / uvheight;

	if (f >= w * h) {
		frame = 0;
		uvoffset.x = 0;
		uvoffset.y = 0;
		return frame;
	}

	/*int ypos = f / w;
	int xpos = f % w;*/

	/*uvoffset.x = xpos * uvwidth;
	uvoffset.y = ypos * uvheight;*/

	frame = f;

	return frame;
}