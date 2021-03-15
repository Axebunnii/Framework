#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <common/camera.h>
#include <common/sprite.h>
#include <common/grid.h>

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void update(float deltaTime);
	void addSprite(Sprite* sprite);
	void addGrid(Grid* grid);

	Camera* camera() { return _camera; };
	std::vector<Sprite*>& sprites() { return _sprites; };

private:
	Camera* _camera;
	std::vector<Sprite*> _sprites;
	std::vector<Grid*> grids;
};

#endif /* SCENE_H */
