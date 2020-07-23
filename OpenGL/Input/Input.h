#pragma once
#include <glfw3.h>
#include "../Camera.h"

//Function to get mouse location and process mouse input.
void MouseCallback(GLFWwindow* window, double xPos, double yPos, Camera& camera);
//Function to process the input of the keyboard.
void ProcessInput(GLFWwindow* window, Camera& camera, float& deltaTime);
//Overload that allows ability to pause and speed adjust.
void ProcessInput(GLFWwindow* window, Camera& camera, float& deltaTime, bool& paused, float& animationSpeed);