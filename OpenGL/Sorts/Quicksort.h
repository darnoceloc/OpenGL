#pragma once
#include <glfw3.h>
#include <glm.hpp>
#include <../Camera.h>
#include <../Geometry/CodedMesh.h>
#include <../Input/Input.h>

//QuickSort without rendering for testing
unsigned int Partition(glm::vec3* colorArray, unsigned int low, unsigned int high) {
    unsigned int pivot = colorArray[low].x*10000.0f + colorArray[low].y*100.0f + colorArray[low].z;
    unsigned int up = low, down = high;

    while(up < down){
        for(int j = up; j < high; j++){
            if(colorArray[up].x*10000.0f + colorArray[up].y*100.0f + colorArray[up].z > pivot) {
                break;
            }
            ++up;
        }
        for(int j = high; j > low; j--){
            if(colorArray[down].x*10000.0f + colorArray[down].y*100.0f + colorArray[down].z < pivot) {
                break;
            }
            --down;
        }
        if(up < down){
            glm::vec3 temp = colorArray[down];
            colorArray[down] = colorArray[up];
            colorArray[up] = temp;
        }
    }

    glm::vec3 temp = colorArray[down];
    colorArray[down] = colorArray[low];
    colorArray[low] = temp;
    return down;
}

void QuickSort(glm::vec3* colorArray, unsigned int low, unsigned int high) {
    if(low < high) {
        unsigned int pivot = Partition(colorArray, low, high);
        QuickSort(colorArray, low, pivot -1);
        QuickSort(colorArray, pivot + 1, high);
    }
}

//QuickSort with rendering for visualization
int partition(glm::vec3* colorArray, int low, int high, int xCoord, int yCoord, int zCoord, unsigned int buffer, 
			   unsigned int programID, GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, Camera& camera, CodedMesh& cube, 
			   float& deltaTime, float& lastTime, float& currentTime, int& frameCount, double&previousFPSTime)
{
     if (glfwWindowShouldClose(window))
     {
         return -1;
     }

     float pivot = colorArray[high].x * 10000.0f + colorArray[high].y * 100.0f + colorArray[high].z;
     int lastElementSwapped = low - 1;
     for(int j = low; j < high; j++)
     {
         if(colorArray[j].x * 10000.0f + colorArray[j].y * 100.0f + colorArray[j].z <= pivot)
         {
             lastElementSwapped++;
             glm::vec3 temp = colorArray[lastElementSwapped];
             colorArray[lastElementSwapped] = colorArray[j];
             colorArray[j] = temp;
         }
     }

     glm::vec3 temp = colorArray[lastElementSwapped + 1];
     colorArray[lastElementSwapped + 1] = colorArray[high];
     colorArray[high] = temp;
     return lastElementSwapped + 1;
}

void QuickSort(glm::vec3* colorArray, int low, int high, int xCoord, int yCoord, int zCoord, unsigned int buffer, 
			   unsigned int programID, GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, Camera& camera, CodedMesh& cube, 
			   float& deltaTime, float& lastTime, float& currentTime, int& frameCount, double&previousFPSTime)
{
    if (counter++ > animationSpeed)
    {
        do
        {
            currentTime = glfwGetTime();
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            frameCount++;
            if (currentTime - previousFPSTime >= 1.0f)
            {
                std::cout << "FPS: " << frameCount << "\r";
                frameCount = 0;
                previousFPSTime = currentTime;
            }

            ProcessInput(window, camera, deltaTime, paused, animationSpeed);

            /* Render here */
            glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            //Added code
            glUseProgram(programID);

            //Pass the projection matrix to shader ( in this case could change every frame )
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projection[0][0]);

            //Camera/view transformation.
            glm::mat4 view = camera.GetViewMatrix();
            glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &view[0][0]);

            glBindVertexArray(cube.GetVAO());
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, xCoord * yCoord * zCoord * sizeof(glm::vec3), &colorArray[0], GL_DYNAMIC_DRAW);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, xCoord * yCoord * zCoord);

            glBindVertexArray(0);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        } while (paused);
        counter = 0;
    }

    if(low < high)
    {
        int pivot = partition(colorArray, low, high, xCoord, yCoord, zCoord, buffer, programID, window, 
                              SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime, frameCount, previousFPSTime); 
        QuickSort(colorArray, low, pivot - 1, xCoord, yCoord, zCoord, buffer, programID, window,
                  SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime, frameCount, previousFPSTime);
        QuickSort(colorArray, pivot + 1, high, xCoord, yCoord, zCoord, buffer, programID, window,
                  SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime, frameCount, previousFPSTime); 
    }
}
