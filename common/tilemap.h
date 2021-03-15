#ifndef TILEMAP_H
#define TILEMAP_H


class TileMap
{
public:
	TileMap(const std::string& filename, int col, int row, float tsize);
	virtual ~TileMap();

private:
	int cols = 0;
	int rows = 0;
	float tileSize = 0;
};


#endif /* TILEMAP_H */