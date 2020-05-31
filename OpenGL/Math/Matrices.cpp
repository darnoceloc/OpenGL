#include "Matrices.h"

glm::vec4 TransformVector(glm::mat4 matrix, glm::vec4 vector)
{
	return matrix * vector;
}