#ifndef TILEMAP_H
#define TILEMAP_H

#include <fstream>
#include <sstream>
#include <vector>

#include "sprite.h"

class TileMap
{
public:
	TileMap();
	virtual ~TileMap();
	void DrawMap(const std::string& filename, char* map, int rows, int cols, float tileSize);
	Sprite* TileMap::GetTile(int x, int y);
	std::vector<Sprite*> maplist;

private:
	int c = 0;
	int r = 0;
	float tsize = 0;
};


#endif /* TILEMAP_H */