#pragma once
#include <glfw3.h>
#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>

void MergeSort(glm::vec3* colorArray, unsigned int start, unsigned int end)
{
	unsigned int mid = (end + start) / 2;
	if (start < end)
	{
		MergeSort(colorArray, start, mid);
		MergeSort(colorArray, mid + 1, end);
	}
	else
	{
		return;
	}

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;
	unsigned int arrayToSortIndex = start;
	unsigned int tempOneSize = mid - start + 1;
	unsigned int tempTwoSize = end - mid;
	glm::vec3* tempArrayOne = new glm::vec3[tempOneSize];
	glm::vec3* tempArrayTwo = new glm::vec3[tempTwoSize];

	for (int i = 0; i < tempOneSize; i++)
	{
		tempArrayOne[i] = colorArray[start + i];
	}
	for (int i = 0; i < tempTwoSize; i++)
	{
		tempArrayTwo[i] = colorArray[mid + 1 + i];
	}
	
	while (arrayToSortIndex <= end)
	{
		if (firstIndex < tempOneSize && secondIndex < tempTwoSize)
		{
			if ((tempArrayOne[firstIndex].x * 10000.0f + tempArrayOne[firstIndex].y * 100.0f + tempArrayOne[firstIndex].z) <
				(tempArrayTwo[secondIndex].x * 10000.0f + tempArrayTwo[secondIndex].y * 100.0f + tempArrayTwo[secondIndex].z))
			{
				colorArray[arrayToSortIndex++] = tempArrayOne[firstIndex++];
			}
			else
			{
				colorArray[arrayToSortIndex++] = tempArrayTwo[secondIndex++];
			}
		}
		else if(firstIndex < tempOneSize)
		{
			colorArray[arrayToSortIndex++] = tempArrayOne[firstIndex++];
		}
		else if (secondIndex < tempTwoSize)
		{
			colorArray[arrayToSortIndex++] = tempArrayTwo[secondIndex++];
		}
	}
}

bool paused = false;
int counter = 0;

void MergeSort(glm::vec3* colorArray, unsigned int start, unsigned int end, int xCoord, int yCoord, int zCoord, unsigned int buffer, 
			   unsigned int programID, GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, Camera &camera, CodedMesh& cube, 
			   float& deltaTime, float& lastTime, float& currentTime)
{
	unsigned int mid = (end + start) / 2;
	if (start < end)
	{
		MergeSort(colorArray, start, mid, xCoord, yCoord, zCoord, buffer, programID,
			window, SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime);
		MergeSort(colorArray, mid + 1, end, xCoord, yCoord, zCoord, buffer, programID,
			window, SCR_WIDTH, SCR_HEIGHT, camera, cube, deltaTime, lastTime, currentTime);
	}
	else
	{
		return;
	}

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;
	unsigned int arrayToSortIndex = start;
	unsigned int tempOneSize = mid - start + 1;
	unsigned int tempTwoSize = end - mid;
	glm::vec3* tempArrayOne = new glm::vec3[tempOneSize];
	glm::vec3* tempArrayTwo = new glm::vec3[tempTwoSize];

	for (int i = 0; i < tempOneSize; i++)
	{
		tempArrayOne[i] = colorArray[start + i];
	}
	for (int i = 0; i < tempTwoSize; i++)
	{
		tempArrayTwo[i] = colorArray[mid + 1 + i];
	}

	while (arrayToSortIndex <= end)
	{
		if (firstIndex < tempOneSize && secondIndex < tempTwoSize)
		{
			if ((tempArrayOne[firstIndex].x * 10000.0f + tempArrayOne[firstIndex].y * 100.0f + tempArrayOne[firstIndex].z) <
				(tempArrayTwo[secondIndex].x * 10000.0f + tempArrayTwo[secondIndex].y * 100.0f + tempArrayTwo[secondIndex].z))
			{
				colorArray[arrayToSortIndex++] = tempArrayOne[firstIndex++];
			}
			else
			{
				colorArray[arrayToSortIndex++] = tempArrayTwo[secondIndex++];
			}
		}
		else if (firstIndex < tempOneSize)
		{
			colorArray[arrayToSortIndex++] = tempArrayOne[firstIndex++];
		}
		else if (secondIndex < tempTwoSize)
		{
			colorArray[arrayToSortIndex++] = tempArrayTwo[secondIndex++];
		}
	}
	//if (counter++ > 13)
	//{
		do
		{
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

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
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, xCoord * yCoord * zCoord * sizeof(glm::vec3), &colorArray[0], GL_DYNAMIC_DRAW);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, xCoord * yCoord * zCoord);


			glBindVertexArray(0);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				paused = !paused;
			}
		} while (paused);
		//counter = 0;
	//}
}