#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/scene.h>
#include <common/camera.h>
#include <common/sprite.h>
#include <common/mesh.h>
#include <common/grid.h>

#include "tilemap.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	void renderScene(Scene* scene);
	void renderSprite(Sprite* sprite);
	GLFWwindow* window() { return _window; };

	float updateDeltaTime();

private:
	int init();

	GLFWwindow* _window;

	Mesh* mesh;

	GLuint loadShaders(
		const std::string& vertex_file_path,
		const std::string& fragment_file_path
	);

	void _renderLine(const glm::mat4 modelMatrix, Grid* line);
	void _renderMap();

	Grid* grid;

	/*inline void _renderMesh(const glm::mat4 modelMatrix, Shader* shader,
		Mesh* mesh, int numverts, GLuint mode, RGBAColor blendcolor);*/
	inline void _renderMesh(const glm::mat4 modelMatrix,
		int numverts, GLuint mode);

	GLuint _programID;

	Camera* _camera; // reference to scene->camera
};

#endif /* RENDERER_H */
