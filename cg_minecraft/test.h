#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

void test() {
	std::vector<glm::vec3> coords;
	std::vector<int> nums;

	coords.push_back(glm::ivec3(1, 2, 3));
	coords.push_back(glm::vec3(7, 8, 4));
	coords.push_back(glm::vec3(5, 6, 5));

	nums.push_back(1);
	nums.push_back(2);
	nums.push_back(3);
	nums.push_back(4);
	nums.push_back(5);
	nums.push_back(6);

	glm::vec3 *cd = coords.data();
	int *nd = nums.data();

	float *cp = (float *)cd;
	int *np = (int *)nd;

	auto begIter = coords.begin() + 1;
	auto endIter = coords.begin() + 2;

	return;
}