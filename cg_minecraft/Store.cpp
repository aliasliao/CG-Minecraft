#include "stdafx.h"
#include <cstdlib>
#include "Store.h"

Cube::Cube(glm::vec3 position, unsigned int type)
{
	this->position = glm::vec3(position);
	this->color = glm::vec3(1.0, 1.0, 1.0);
	this->type = 0;
	unsigned int faces[] = { 0, 1, 2, 3, 4, 5 };  // f, b, l, r, t, b
	this->faces = std::set<unsigned int>(faces, faces+6);
}
Cube::Cube(glm::vec3 position, glm::vec3 color, unsigned int type)
{
	new (this) Cube(position, type);
	this->color = glm::vec3(color);
}

Store::Store()
{
	this->cubes = std::vector<Cube>();

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	GLfloat* vertices = this->getVertices();
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // to be formatted
}

bool Store::addObject(Cube cube)
{
	if (cube.position.z < 0) {
		return false;
	}

	for (auto &c : this->cubes) {
		if (c.position == cube.position) {
			return false;
		}
	}

	for (auto &c : this->cubes) {
		glm::vec3 distance = c.position - cube.position;
		unsigned int cIndex=6, cubeIndex;

		if (distance == glm::vec3(1, 0, 0)) {
			cIndex = 1;
			cubeIndex = 0;
		}
		else if (distance == glm::vec3(-1, 0, 0)) {
			cIndex = 0;
			cubeIndex = 1;
		}
		else if (distance == glm::vec3(0, 1, 0)) {
			cIndex = 2;
			cubeIndex = 3;
		}
		else if (distance == glm::vec3(0, -1, 0)) {
			cIndex = 3;
			cubeIndex = 2;
		}
		else if (distance == glm::vec3(0, 0, 1)) {
			cIndex = 5;
			cubeIndex = 4;
		}
		else if (distance == glm::vec3(0, 0, -1)) {
			cIndex = 4;
			cubeIndex = 5;
		}

		if (cIndex != 6) {
			c.faces.erase(c.faces.find(cIndex));
			cube.faces.erase(cube.faces.find(cubeIndex));
		}
	}

	this->cubes.push_back(cube);

	return true;
}

bool Store::removeObject(glm::vec3 position)
{
	for (auto it = this->cubes.begin(); it != this->cubes.end(); it++) {
		if ((*it).position == position) {
			this->cubes.erase(it);

			return true;
		}
	}

	return false;
}

GLfloat * Store::getVertices()
{
	int faceCnt = 0;
	
	for (auto &c : this->cubes) {
		faceCnt += c.faces.size();
	}

	int vertCnt = faceCnt * 2 * 3;
	GLfloat *vertices = (GLfloat *)malloc(vertCnt * 9 * sizeof(GLfloat));

	for (auto &c : this->cubes) {
		GLfloat
			px = c.position.x,
			py = c.position.y,
			pz = c.position.z,
			cr = c.color.r,
			cg = c.color.g,
			cb = c.color.b,
			t = c.type;

		GLfloat allVert[6][6 * 9] = {
			{
				px+1, py  , pz  , cr, cg, cb, t, 0, 0,
				px+1, py+1, pz  , cr, cg, cb, t, 1, 0,
				px+1, py+1, pz+1, cr, cg, cb, t, 1, 1,
				px+1, py+1, pz+1, cr, cg, cb, t, 1, 1,
				px+1, py  , pz+1, cr, cg, cb, t, 0, 1,
				px+1, py  , pz  , cr, cg, cb, t, 0, 0,
			},
		};
	}

	return nullptr;
}