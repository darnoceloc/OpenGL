#pragma once
#include <GLFW/glfw3.h>

//Storage class to store the different shapes' vertices and sizes.
//To add shape: 1) Add to enum Shape. 2) Add the vertice array and size of shape in bytes. 3) Add in the switch statesment in GetShapeVertices
//and GetShapeByteSize.
//Things to do: 1)Might combine the shapes into a struct.
class ShapeVertices
{
public:
	static enum Shape
	{
		Cube = 0
	};

	/*******CUBE*******/
	//Simple 1 unit cube, with 108 total floats equaling 36 vertices. 108 * 4 = 432 bytes in total.
	static const GLfloat cubeVertices[];
	static unsigned const int cubeSizeInBytes = 432;

	static const GLfloat* GetShapeVertices(ShapeVertices::Shape shape);
	static unsigned const int GetShapeByteSize(ShapeVertices::Shape shape);
};