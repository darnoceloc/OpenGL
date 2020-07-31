#pragma once
#include <glfw3.h>
#include <glm.hpp>
#include <algorithm>

//Simple class to shuffle the color array.
class Randomizer
{
public:
	static void Randomize(glm::vec3* colors, unsigned int colorBuffer, int cubeGridXCoord, int cubeGridYCoord, int cubeGridZCoord)
	{
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

		//Shuffle the color array.
		std::random_shuffle(&colors[0], &colors[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord]);
	}
};
