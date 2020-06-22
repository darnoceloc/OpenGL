#pragma once
#include <glm.hpp>

unsigned int MergeSort(glm::vec3* colorArray, unsigned int start, unsigned int end)
{
	unsigned int mid = (end + start) / 2;
	MergeSort(colorArray, start, mid);
	MergeSort(colorArray, mid + 1, end);
}