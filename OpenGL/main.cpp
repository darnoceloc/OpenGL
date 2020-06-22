/******************************************************************************************************************************************************
*
* Created By:
* Course: COP3530
* Project 3: Comparison Between Sorting Algorithms(title work in progress)
* Professor: Kapoor
*
* Last Update: 19 June 2020 (Got shape data in a class)
*
* Need to do: abstract the rendering, might adjust input movement.
*******************************************************************************************************************************************************/

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <algorithm>

#include "Shaders/Functions/loadShader.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Geometry/ShapeVertices.h"
#include "Geometry/CodedMesh.h"
#include "Renderer/Renderer.h"

//Function used to resize the window appropriately.
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
inline void Mouse(GLFWwindow* window, double xPos, double yPos);

//Global screen settings.
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

//Global camera variables.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Global timing variables.
float deltaTime = 0.0f;         //Time difference of current frame and last frame.
float lastTime = 0.0f;         //Keeps track of the time of the last frame. Used to calculate deltaTime.

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed." << std::endl;
        return -1;
    }

    /*       OpenGLtutorials.org tutorial       */
    //Triangle doesn't show if other 3 are uncommented
    glfwWindowHint(GLFW_SAMPLES, 4);                                    //4x antialiasing
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                      //OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      //We don't want the old OpenGL
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    /*       OpenGLtutorials.org tutorial end       */

    GLFWwindow* window;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1024, "Window of the Gods!", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW window creation failed." << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Added code
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(window, Mouse);

    //Tells GLFW to capture our mouse.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Check if GLEW is initialized
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed to initialize." << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Compile and load shaders and store the program id
    GLuint programID = LoadShaders("Shaders/Vertex/CameraShader.vert", "Shaders/Fragment/SimpleFragmentShader.frag");

    //Prints the GLEW and openGL versions
    std::cout << "Using GLEW version :" << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Using openGL version: " << glGetString(GL_VERSION) << std::endl;

    //INSTANCING TEST
    Renderer renderer;
    //The coords are for placing the cubes, there will be x * y * z cubes.
    int cubeGridXCoord = 20;
    int cubeGridYCoord = 20;
    int cubeGridZCoord = 20;
    glm::mat4* modelMatrices = new glm::mat4[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord];
    renderer.SetModelMatrix(modelMatrices, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord, 0.02f, 0.01f, 0, 0, 0);

    glm::vec3* colors = new glm::vec3[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord];
    int currentIndex = 0;
    for (unsigned int i = 0; i < cubeGridXCoord; i++)
    {
        for (unsigned int j = 0; j < cubeGridYCoord; j++)
        {
            for (unsigned int k = 0; k < cubeGridZCoord; k++)
            {
                colors[currentIndex++] = glm::vec3((((float)255 / (cubeGridXCoord - 1)) * i) / 255.0f, (((float)255 / (cubeGridXCoord - 1)) * j) / 255.0f, (((float)255 / (cubeGridXCoord - 1)) * k) / 255.0f);
            }
        }
    }
    

    //Mesh to hold the vertex, VAO, and VBO data.
    CodedMesh cube(ShapeVertices::Cube);
    glBindVertexArray(cube.GetVAO());

    unsigned int colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

    std::random_shuffle(&colors[0], &colors[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord]);

    /*currentIndex = 0;
    int indexToCheck = 0;
    for (unsigned int i = 0; i < cubeGridXCoord; i++)
    {
        for (unsigned int j = 0; j < cubeGridYCoord; j++)
        {
            for (unsigned int k = 0; k < cubeGridZCoord; k++)
            {
                while (indexToCheck < 27000)
                {
                    if (colors[indexToCheck] == glm::vec3((((float)255 / (cubeGridXCoord - 1)) * i) / 255.0f, (((float)255 / (cubeGridXCoord - 1)) * j) / 255.0f, (((float)255 / (cubeGridXCoord - 1)) * k) / 255.0f))
                    {
                        glm::vec3 temp = colors[currentIndex];
                        colors[currentIndex++] = colors[indexToCheck];
                        colors[indexToCheck] = temp;
                    }
                    indexToCheck++;
                }
                indexToCheck = 0;
            }
        }
    }*/



    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(1, 1);

    unsigned int matricesBuffer;
    glGenBuffers(1, &matricesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // vertex attributes
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindVertexArray(0);

    double previousFPSTime = glfwGetTime();
    int frameCount = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Get the time variables and display fps
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime= currentTime;
        frameCount++;
        if (currentTime - previousFPSTime >= 1.0f)
        {
            std::cout << "FPS: " << frameCount << "\r";
            frameCount = 0;
            previousFPSTime = currentTime;
        }

        //Input
        ProcessInput(window, camera, deltaTime);


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
        //glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord);
        glBindVertexArray(0);

        //Added code end

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDisableVertexAttribArray(0);
    glfwTerminate();
    return 0;
}

//Updates window when changed by OS or user. 
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Make sure the viewport matches the new window dimensions.
    glViewport(0, 0, width, height);
}

inline void Mouse(GLFWwindow* window, double xPos, double yPos)
{
    MouseCallback(window, xPos, yPos, camera);
}