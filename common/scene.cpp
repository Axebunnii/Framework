#include <iostream>
#include <common/scene.h>
#include <common/config.h>

Scene::Scene()
{
	_camera = new Camera();
}

Scene::~Scene()
{
	// Delete all the sprites
	for (size_t i = 0; i < _sprites.size(); i++)
	{
		delete _sprites[i];
		_sprites[i] = nullptr;
	}
	_sprites.clear();

	// Delete all the grids
	for (size_t i = 0; i < _lines.size(); i++)
	{
		delete _lines[i];
		_lines[i] = nullptr;
	}
	_lines.clear();

	// Delete camera
	delete _camera;
}

void Scene::update(float deltaTime)
{
	
}

void Scene::addSprite(Sprite* sprite)
{
	_sprites.push_back(sprite);
}

void Scene::addLine(Line* line) {
	_lines.push_back(line);
}