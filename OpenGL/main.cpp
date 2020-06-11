/******************************************************************************************************************************
* Last Update: 11 June 2020 (added the up input for space: makes the camera move up in world space.)
*
*
******************************************************************************************************************************/

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shaders/Functions/loadShader.h"
#include "Camera.h"
#include "Input/Input.h"

//Function used to resize the window appropriately.
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
inline void Mouse(GLFWwindow* window, double xPos, double yPos);

//Global screen settings.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Global camera variables.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Global timing variables.
float deltaTime = 0.0f;         //Time difference of current frame and last frame.
float lastFrame = 0.0f;         //Keeps track of the time of the last frame. Used to calculate deltaTime.

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
    
    //Compile and load shaders and store the program id
    GLuint programID = LoadShaders("Shaders/Vertex/CameraShader.vert", "Shaders/Fragment/SimpleFragmentShader.frag");

    //Prints the GLEW and openGL versions
    std::cout << "Using GLEW version :" << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Using openGL version: " << glGetString(GL_VERSION) << std::endl;

    /*
      An array of 3 vectors which represents 3 vertices; 6 to make a rectangle; Each segment represents a face of a cube, made of two triangles; 
      Looking at the face, the first vertex of each triangle is top left, triangle formed counter-clockwise; First vertex on top is (-0.5, 0.5, -0.5);
      First vertex on bottom is (-0.5, -0.5, 0.5);
    
        1         3
      4    
             
               
                  2
      5         6
    */
    static const GLfloat g_vertex_buffer_data[] =
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
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         //Bottom
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
    };

    //World space position of our cube.
    glm::vec3 cubePosition[] =
    {   
        //4
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f, -1.0f),
        glm::vec3( 0.0f,  0.0f, -2.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f),
        glm::vec3( 1.0f,  0.0f, -1.0f),
        glm::vec3(-1.0f,  0.0f, -1.0f),
        glm::vec3(-2.0f,  0.0f, -1.0f),
        glm::vec3(-2.0f,  0.0f, -2.0f),
        glm::vec3(-2.0f,  0.0f, -3.0f),
        //2
        glm::vec3( 4.0f,  0.0f,  0.0f),
        glm::vec3( 3.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  0.0f,  0.0f),
        glm::vec3( 3.0f,  0.0f, -1.0f),
        glm::vec3( 4.0f,  0.0f, -2.0f),
        glm::vec3( 4.0f,  0.0f, -3.0f),
        glm::vec3( 3.0f,  0.0f, -3.0f),
        glm::vec3( 2.0f,  0.0f, -3.0f),
        //0
        glm::vec3( 8.0f,  0.0f,  0.0f),
        glm::vec3( 8.0f,  0.0f, -1.0f),
        glm::vec3( 8.0f,  0.0f, -2.0f),
        glm::vec3( 8.0f,  0.0f, -3.0f),
        glm::vec3( 6.0f,  0.0f,  0.0f),
        glm::vec3( 6.0f,  0.0f, -1.0f),
        glm::vec3( 6.0f,  0.0f, -2.0f),
        glm::vec3( 6.0f,  0.0f, -3.0f),
        glm::vec3( 7.0f,  0.0f,  0.0f),
        glm::vec3( 7.0f,  0.0f, -3.0f),
    };

    //Identify vertex buffer
    GLuint vertexbuffer;
    //Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //Give the vertices to OpenGL
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //Added code end

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Get the time variables
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        ProcessInput(window, camera, deltaTime);


        /* Render here */
        glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Added code
        glUseProgram(programID);

        //First attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glVertexAttribPointer
            (
                0,              //attribute 0. No reason 0, but must match layout in shader. 
                3,              //size
                GL_FLOAT,       //type
                GL_FALSE,       //normalized?
                0,              //stride
                (void*)0        //array buffer offset
            );

        //Pass the projection matrix to shader ( in this case could change every frame )
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projection[0][0]);

        //Camera/view transformation.
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &view[0][0]);
        
        for (unsigned int i = 0; i < 27; i++)
        {
            //Calculate model matrix and initialize.
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition[i]);
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &model[0][0]);

            //Draw the triangle
            glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices = one triangle, 6 = one face, 36 = one cube;
        }

        glDisableVertexAttribArray(0);

        //Added code end

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
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