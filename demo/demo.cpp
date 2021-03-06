// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <common/config.h>
#include <common/renderer.h>
#include <common/camera.h>
#include <common/scene.h>
#include <common/sprite.h>
#include <common/Tilemap.h>

int main(void)
{
	Renderer renderer;

	Scene* scene = new Scene();

	TileMap* tilemap = new TileMap();
	
	//map->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0.0f);

	//TileMap* map[2][4]{
	char map[4]{
		0, 1,
		1, 0
	};
	tilemap->DrawMap("assets/kingkong.tga", map, 2, 2, 260);

	scene->addTilemap(tilemap);

	/*Sprite* tilesheet = new Sprite("assets/tilesheet.tga");
	scene->addSprite(tilesheet);
	tilesheet->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0.0f);*/

	do {
		// get deltaTime and update camera
		float dt = renderer.updateDeltaTime();
		scene->camera()->computeViewMatrixFromInput(renderer.window(), dt);

		// Update the scene
		scene->update(dt); // TODO make subclass for Scene (and make Scene abstract?)
		//rgba->rotation += dt; // for now, do it here

		// Render the scene
		renderer.renderScene(scene);
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(renderer.window(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(renderer.window()) == 0);

	delete scene;

	return 0;
}