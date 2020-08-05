#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera.h"
#include "../Geometry/CodedMesh.h"

unsigned int partition(glm::vec3* colorArray, unsigned int low, unsigned int high) {
    unsigned int lowIndex = low - 1;
    unsigned int pivot = colorArray[high].x*10000.0f + colorArray[high].y*100.0f + colorArray[high].z;

    for(int i = low; i < high; ++i) {
        if(colorArray[i].x*10000.0f + colorArray[i].y*100.0f + colorArray[i].z <= pivot) {
            ++lowIndex;
            glm::vec3 temp = colorArray[lowIndex];
            colorArray[lowIndex] = colorArray[i];
            colorArray[i] = temp;
        }
    }

    ++lowIndex;
    glm::vec3 temp = colorArray[lowIndex];
    colorArray[lowIndex] = colorArray[high];
    colorArray[high] = temp;
    return lowIndex;
}

void quickSortImplemented(glm::vec3* colorArray, unsigned int low, unsigned int high) {
    if(low < high) {
        unsigned int pi = partition(colorArray, low, high);

        quickSort(colorArray, low);
        quickSort(colorArray, high);
    }
}

void quickSort(glm::vec3* colorArray, unsigned int length) {
    quickSortImplemented(colorArray, 0, length - 1);
}



void quickSort(glm::vec3* colorArray, unsigned int start, unsigned int end, int xCoord, int yCoord, int zCoord, unsigned int buffer, 
			   unsigned int programID, GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, Camera& camera, CodedMesh& cube, 
			   float& deltaTime, float& lastTime, float& currentTime){

               }