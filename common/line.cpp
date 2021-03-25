#include <iostream>

#include <common/line.h>

Line::Line() {
	
}

Line::~Line() {

}

void Line::DrawLine(float x1, float y1, float x2, float y2) {
	vertexLinePoints = {
		x1, y1,
		x2, y2,
	};
}