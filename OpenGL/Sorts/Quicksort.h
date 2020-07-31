#pragma once
#include <glfw3.h>
#include <glm.hpp>

unsigned int Partition(glm::vec3* colorArray, unsigned int low, unsigned int high)
{
    unsigned int lowIndex = low - 1;

    unsigned int pivot = colorArray[high].x * 10000.0f + colorArray[high].y * 100.0f + colorArray[high].z;
    for (int i = low; i < high; ++i) {

        if (colorArray[i].x * 10000.0f + colorArray[i].y * 100.0f + colorArray[i].z <= pivot)
        {
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

void QuickSortImplemented(glm::vec3* colorArray, unsigned int low, unsigned int high)
{
    if (low < high)
    {
        unsigned int pi = Partition(colorArray, low, high);

        QuickSort(colorArray, low);

        QuickSort(colorArray, high);

    }

}

void QuickSort(glm::vec3* colorArray, unsigned int length)
{
    QuickSortImplemented(colorArray, 0, length - 1);
}

bool paused = false;
int counter = 0;
float animationSpeed = 1;

unsigned int partition(glm::vec3* colorArray, unsigned int low, unsigned int high, int xCoord, int yCoord, int zCoord, unsigned int buffer,
    unsigned int programID, GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, Camera& camera, CodedMesh& cube,
    float& deltaTime, float& lastTime, float& currentTime, int& frameCount, double& previousFPSTime)
{
    if (glfwWindowShouldClose(window))
    {
        return;
    }

    unsigned int lowIndex = low - 1;
    unsigned int pivot = colorArray[high].x * 10000.0f + colorArray[high].y * 100.0f + colorArray[high].z;

    for (int i = low; i < high; ++i)
    {
        if (colorArray[i].x * 10000.0f + colorArray[i].y * 100.0f + colorArray[i].z <= pivot)
        {
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

            ​//Camera/view transformation.
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