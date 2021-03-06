#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <common/camera.h>
#include <common/sprite.h>
#include <common/grid.h>
#include <common/line.h>
#include <common/tilemap.h>

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void update(float deltaTime);
	void addSprite(Sprite* sprite);
	void addLine(Line* line);
	void addTilemap(TileMap* tilemap);

	Camera* camera() { return _camera; };
	std::vector<Sprite*>& sprites() { return _sprites; };
	std::vector<Line*>& lines() { return _lines; };

private:
	Camera* _camera;
	std::vector<Sprite*> _sprites;
	std::vector<Line*> _lines;
	std::vector<TileMap*> maps;
};

#endif /* SCENE_H */
