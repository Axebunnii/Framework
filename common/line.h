#ifndef LINE_H
#define LINE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Line {
public:
	Line();
	virtual ~Line();
	void DrawLine(float x1, float y1, float x2, float y2);

	std::vector<GLfloat> vertexLinePoints;

private:

};

#endif /* LINE_H */