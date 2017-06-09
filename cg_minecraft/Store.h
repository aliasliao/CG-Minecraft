#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "define.h"

class Store {
	bool hasGround;

	std::vector<glm::ivec3> vertices;
	std::vector<glm::ivec3> normals;
	std::vector<glm::ivec3> textures;  // tex_x, tex_y, tex_index
	std::vector<glm::uvec3> elements;

	GLuint vao, vertexVbo, normalVbo, textureVbo, elementEbo;

	void initBuffers();  // vertex attribute point
	void addGround();
	void upload();  // upload buffer to gpu
	int getCubeIndex(const glm::ivec3 & position);  // 0 for ground
	bool loadState(const std::string &fileName);
	bool loadExternal(const std::string &fileName);
	
public:
	Store(bool needGround=false);  // whether the store need a ground
	Store(const std::string &fileName, bool hasGround=true);  // load from file, whether the obj has a ground
	bool saveState(const std::string &fileName);  // save to file
	bool addCube(const glm::ivec3 &position, const CubeType type);
	bool removeCube(const glm::ivec3 &position);
	void draw();
};
