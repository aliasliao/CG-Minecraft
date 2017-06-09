#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "define.h"

class Store {
	bool isGround;

	std::vector<glm::ivec3> vertices;
	std::vector<glm::ivec3> normals;
	std::vector<glm::ivec3> textures;  // tex_x, tex_y, tex_index
	std::vector<glm::uvec3> elements;

	GLuint vao, vertexVbo, normalVbo, textureVbo, elementEbo;

	void initBuffers();  // vertex attribute point
	void addGround();
	void upload();  // upload buffer to gpu
	bool isOccupied(const glm::ivec3 &position);
	int getCubeIndex(const glm::ivec3 & position);  // 0 for ground
	
public:
	Store(bool isGround = false);
	Store(const std::string fileName);  // load from file
	bool saveFile(const std::string &fileName);  // save to file
	bool addCube(const glm::ivec3 &position, const CubeType type, const Direction zFaceDirection=FRONT);
	bool removeCube(const glm::ivec3 &position);
	void draw();
};
