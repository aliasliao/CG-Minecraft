#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <set>

struct Cube {
	glm::vec3 position;  // integer
	glm::vec3 color;
	unsigned int type;
	std::set<unsigned int> faces;

	Cube(glm::vec3 position, unsigned int type);
	Cube(glm::vec3 position, glm::vec3 color, unsigned int type);
};

class Store {
	std::vector<Cube> cubes;
	GLuint vao;
	GLuint vbo;

	GLfloat* getVertices();
	
public:
	Store();
	Store(std::string fileName);
	bool addObject(Cube cube);
	bool removeObject(glm::vec3 position);
	bool saveFile();
};
