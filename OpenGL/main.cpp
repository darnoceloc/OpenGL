/******************************************************************************************************************************************************
*
* Created By:
* Course: COP3530
* Project 3: Comparison Between Sorting Algorithms(title work in progress)
* Professor: Kapoor
*
* Last Update: 31 July 2020 (added speed controls and comments)
*
* Need to do: abstract the rendering, fix process input, add controls to switch between merge and quick.

  Citations: Some code regarding rendering and controls were adapted from https://learnopengl.com/Introduction, accessed last on 7/30/2020,
             Author: Joey de Vries
             https://learnopengl.com/Getting-started/Textures - used to load menu texture into rectangle
*******************************************************************************************************************************************************/
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shaders/Functions/loadShader.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Geometry/ShapeVertices.h"
#include "Geometry/CodedMesh.h"
#include "Renderer/Renderer.h"
#include "Sorts/MergeSort.h"
#include "Sorts/Quicksort.h"
#include "Sorts/Randomizer.h"

#include <vector>

//Function used to resize the window appropriately.
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
inline void Mouse(GLFWwindow* window, double xPos, double yPos);

//Callbacks for menu and pause.
void MenuCallback(GLFWwindow* window, int key, int code, int action, int mods);

//Global screen settings.
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

//Global camera variables.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Global timing variables.
float deltaTime = 0.0f;        //Time difference of current frame and last frame.
float lastTime = 0.0f;         //Keeps track of the time of the last frame. Used to calculate deltaTime.

//Bool to toggle menu
bool menu = false;
bool isMerge = true;

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed." << std::endl;
        return -1;
    }

    //Triangle doesn't show if other 3 are uncommented
    glfwWindowHint(GLFW_SAMPLES, 4);                                    //4x antialiasing

    GLFWwindow* window;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1024, "BC2 Final Project", NULL, NULL);
    
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
    glfwSetKeyCallback(window, MenuCallback);

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
    GLuint programIDTwo = LoadShaders("Shaders/Vertex/CameraShaderTwo.vert", "Shaders/Fragment/SimpleFragmentShaderTwo.frag");

    //Prints the GLEW and openGL versions
    std::cout << "Using GLEW version :" << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Using openGL version: " << glGetString(GL_VERSION) << std::endl;


    ///////////////////MENU TESTING

        /*Menu Texture*/
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f,  0.0f,   /*1.0f, 0.0f, 0.0f,*/  1.0f, 1.0f, // top right
         1.0f,  0.0f,  0.0f,   /*1.0f, 0.0f, 0.0f,*/  1.0f, 0.0f, // bottom right
         0.0f,  0.0f,  0.0f,   /*1.0f, 0.0f, 0.0f,*/  0.0f, 0.0f, // bottom left
         0.0f,  1.0f,  0.0f,   /*1.0f, 0.0f, 0.0f,*/  0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        2, 1, 0, // first triangle
        0, 3, 2  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("menu.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ///////////////////////////END TEST


    //INSTANCING
    Renderer renderer;
    //The coords are for placing the cubes, there will be x * y * z cubes.
    int cubeGridXCoord = 50;
    int cubeGridYCoord = 50;
    int cubeGridZCoord = 50;
    //Model matrices for each of the smaller cubes.
    glm::mat4* modelMatrices = new glm::mat4[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord];
    renderer.SetModelMatrix(modelMatrices, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord, 0.02f, 0.01f, 0, 0, 0);

    //Create and populate the color array.
    glm::vec3* colors = new glm::vec3[cubeGridXCoord * cubeGridYCoord * cubeGridZCoord];
    int currentIndex = 0;
    for (unsigned int i = 0; i < cubeGridXCoord; i++)
    {
        for (unsigned int j = 0; j < cubeGridYCoord; j++)
        {
            for (unsigned int k = 0; k < cubeGridZCoord; k++)
            {
                colors[currentIndex++] = glm::vec3((((float)255 / (cubeGridXCoord - 1)) * i) / 255.0f, (((float)255 / (cubeGridYCoord - 1)) * j) / 255.0f, (((float)255 / (cubeGridZCoord - 1)) * k) / 255.0f);
            }
        }
    }


    //Mesh to hold the vertex, VAO, and VBO data.
    CodedMesh cube(ShapeVertices::Cube);
    glBindVertexArray(cube.GetVAO());

    //Create, bind, and push the color buffer to the GPU.
    unsigned int colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

    //Shuffle the color array.
    Randomizer::Randomize(colors, colorBuffer, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord);

    //Will sort the color array before rendering, used for testing.
    //MergeSort(colors, 0, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord - 1);

    //Push the new randomized color buffer to GPU.
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(1, 1);

    //Assign and push matricesBuffer to GPU.
    unsigned int matricesBuffer;
    glGenBuffers(1, &matricesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    //Vertex attributes for the model matrices
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

    //Time variables used for rendering and FPS. isSorted used to enter sort loop or not.
    double previousFPSTime = glfwGetTime();
    int frameCount = 0;
    bool isSorted = false;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Get the time variables and display fps
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
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

        //// render container for menu
        ////ourShader.use();
        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //Use the shaders chosen.
        glUseProgram(programID);

        //Pass the projection matrix to shader ( in this case could change every frame )
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projection[0][0]);

        //Camera/view transformation.
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &view[0][0]);

        //Bind the appropriate VAO and then call merge sort to sort during runtime.
        glBindVertexArray(cube.GetVAO());
        if (!isSorted && !paused)
        {
            isSorted = true;
            if (isMerge)
            {
                MergeSort(colors, 0, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord - 1, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord, colorBuffer, programID,
                    window, SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime, frameCount, previousFPSTime);
            }
            else
            {
                QuickSort(colors, 0, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord - 1, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord, colorBuffer, programID,
                    window, SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime, frameCount, previousFPSTime);
            }
            
            //Push the final sorted color array, and then draw it.
            glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
            glBufferData(GL_ARRAY_BUFFER, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord * sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord);
        }

        //If the r key is pressed while a sort isn't being performed, the color array will be reshuffled.
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            Randomizer::Randomize(colors, colorBuffer, cubeGridXCoord, cubeGridYCoord, cubeGridZCoord);
            isSorted = false;
            paused = true;
            counter = 2;
        }
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, cubeGridXCoord * cubeGridYCoord * cubeGridZCoord);
        
        if (menu)
        {
            //Use the shaders chosen.
            glUseProgram(programIDTwo);

            //Pass the projection matrix to shader ( in this case could change every frame )
            glm::mat4 projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glUniformMatrix4fv(glGetUniformLocation(programIDTwo, "projection"), 1, GL_FALSE, &projection2[0][0]);

            //Camera/view transformation.
            glm::mat4 view2 = camera.GetViewMatrix();
            glUniformMatrix4fv(glGetUniformLocation(programIDTwo, "view"), 1, GL_FALSE, &view2[0][0]);

            glm::mat4 model = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(programIDTwo, "model"), 1, GL_FALSE, &model[0][0]);

            glm::vec3 position = { camera.position.x -0.5f, camera.position.y -0.5f, camera.position.z - 1.2f};
            glUniform3fv(glGetUniformLocation(programIDTwo, "position"), 1, &position[0]);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }


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

void MenuCallback(GLFWwindow* window, int key, int code, int action, int mods)
{
    //If the m key is pressed while not sorting it will toggle menu.
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        menu = !menu;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        paused = !paused;
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        isMerge = true;
        std::cout << "Now using merge sort." << std::endl;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        isMerge = false;
        std::cout << "Now using quick sort." << std::endl;
    }
}
