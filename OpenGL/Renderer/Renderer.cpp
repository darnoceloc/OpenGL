#include "Renderer.h"

void Renderer::SetModelMatrix(glm::mat4* matrices, int xCoord, int yCoord, int zCoord, float displacement, float scale,
                              int instancesXPos, int instancesYPos, int instancesZPos)
{
    //Keeps track of the position in the array in which the particular model matrix will be stored.
    unsigned int currentIndex = 0;

    for (int i = 0; i < xCoord; i++)
    {
        for (int j = 0; j < yCoord; j++)
        {
            for (int k = 0; k < zCoord; k++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)i * displacement + (instancesXPos - xCoord / 2 * displacement), 
                                                        (float)j * displacement + (instancesYPos - yCoord / 2 * displacement), 
                                                        (float)k * displacement + (instancesZPos - xCoord / 2 * displacement)));
                model = glm::scale(model, glm::vec3(scale));
                matrices[currentIndex++] = model;
            }
        }
    }
}