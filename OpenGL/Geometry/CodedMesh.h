#pragma once
#include <glew.h>
#include <glfw3.h>
#include "ShapeVertices.h"

class CodedMesh
{
private:
	unsigned int VAO;
	unsigned int VBO;

public:
	unsigned int GetVAO();
	CodedMesh(ShapeVertices::Shape shape);
};