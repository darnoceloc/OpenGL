#include "ShapeVertices.h"

const GLfloat* ShapeVertices::GetShapeVertices(ShapeVertices::Shape shape)
{
    switch (shape)
    {
        case ShapeVertices::Cube: return ShapeVertices::cubeVertices;
    }
}
unsigned const int ShapeVertices::GetShapeByteSize(ShapeVertices::Shape shape)
{
    switch (shape)
    {
        case ShapeVertices::Cube: return ShapeVertices::cubeSizeInBytes;
    }
}

/*
      *******CUBE*******
      An array of 3 vectors which represents 3 vertices; 6 to make a rectangle; Each segment represents a face of a cube, made of two triangles;
      Looking at the face, the first vertex of each triangle is top left, triangle formed counter-clockwise; First vertex on top is (-0.5, 0.5, -0.5);
      First vertex on bottom is (-0.5, -0.5, 0.5);

        1         3
      4


                  2
      5         6
*/
const GLfloat ShapeVertices::cubeVertices[] =
{
	//Front
   -0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    //Right
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    //Back
    0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
    //Left
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
    //Top
   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    //Bottom
   -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
};
