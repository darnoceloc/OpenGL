#pragma once

//#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

enum CameraMovement
{
	FORWARD,
	RIGHT,
	BACKWARD,
	LEFT,
	UP
};

//Default camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
	//Camera attributes
	glm::vec3 position;
	//Camera axes
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float pitch;
	float yaw;

	float movementSpeed;
	float sensitivity;

	//Camera constructor with defaults; Front, mouse sensitivity, movement speed does not change
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
		   : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), sensitivity(SENSITIVITY)
	{
		this->position = position;
		this->worldUp  = up;
		this->yaw      = yaw;
		this->pitch    = pitch;
		UpdateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), sensitivity(SENSITIVITY)
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = yaw;
		pitch = pitch;
		UpdateCameraVectors();
	}

	//Return the view matrix; Uses glm's lookat function to compose the view matrix; Takes 3 parameters: camera's postion, direction the 
	//camera is looking, and the local up axis of the camera 
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	//Processes the input from keyboard.
	void ProcessKeyboard(CameraMovement direction, float deltaTime)
	{ 
		std::cout << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			position += front * velocity;
		}
		if (direction == BACKWARD)
		{
			position -= front * velocity;
		}
		if (direction == LEFT)
		{
			position -= right * velocity;
		}
		if (direction == RIGHT)
		{
			position += right * velocity;
		}
		if (direction == UP)
		{
			position += worldUp * velocity;
		}
	}
	
	//Processes the mouse's movement; Takes in 2 parameters: the mouse's x and y positions in relation to the center of the window
	void ProcessMouseMovement(float xPos, float yPos)
	{
		xPos *= sensitivity;
		yPos *= sensitivity;

		yaw   += xPos;
		pitch += yPos;

		//Clamps the pitch between 89.0f degrees and -89.0f degrees; If it reaches 90.0f or -90.0f, it can cause the lookat matrix to flip
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
		UpdateCameraVectors();
	}

private:
	//Calculate the front vector from yaw and pitch; Calculate the right vector from front vector and the world up vector; Calculate the camera's 
	//up vector from the right and front vectors
	void UpdateCameraVectors()
	{
		//Non normalized front vector; x and z affected by pitch to keep movement speed consistent when looking up or down
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		//Set camera's front as the the new normalized front vector
		this->front = glm::normalize(front);
		//Calculates right vector with the normalized cross product of the front and world up vectors
		this->right = glm::normalize(glm::cross(this->front, worldUp));
		//Calculates up vector with the normalized cross product of the right and up vectors
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};