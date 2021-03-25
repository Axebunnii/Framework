#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Grid {
public:
	Grid(/*int xSize, int ySize, int cellSize*/);
	virtual ~Grid();

	void drawGrid(int xSize, int ySize, int cellSize);
	void line(int x1, int y1, int x2, int y2);
	bool dynamic() { return _dynamic; };

	bool closed() { return _closed; };
	void closed(bool c) { _closed = c; };

private:
	unsigned int rows = 1;
	unsigned int cols = 1;

	unsigned int cell = 1;

	bool _dynamic;
	bool _closed = false;
};


#endif /* GRID_H */