#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Renderer
{
public:
	//Sets the model matrices, renderering x * y * z instances of the model in a cube shape, each displacement amount of distance away from the 
	//last centered at the instances position.
	void SetModelMatrix(glm::mat4* matrices, int xCoord, int yCoord, int zCoord, float displacement, float scale = 1,
		                int instancesXPos = 0, int instancesYPos = 0, int instancesZPos = 0);
	void SetViewMatrix();
	void SetProjectionMatrix();
};