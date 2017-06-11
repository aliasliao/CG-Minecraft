#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "define.h"

class Store {
	bool isGround;

	std::vector<glm::vec3> vertices;  // x,y,z are p+[0,1]
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> textures;  // tex_x, tex_y, tex_index, only for cube
	std::vector<glm::uvec3> elements;

	GLuint vao, vertexVbo, normalVbo, textureVbo, elementEbo;

	void initBuffers();  // vertex attribute point
	bool addGround();
	void upload();  // upload buffer to gpu
	int getCubeIndex(const glm::ivec3 & position);  // 0 for ground
	bool loadState(const std::string &fileName);
	bool loadExternal(const std::string &fileName);
	
public:
	Store(bool isGround=false);  // whether the store is a ground
	Store(const std::string &fileName, bool isExternal=false);  // load from file, all file has no ground
	bool saveState(const std::string &fileName);  // save to file
	bool addCube(const glm::ivec3 &position, const CubeType type);
	bool removeCube(const glm::ivec3 &position);
	void draw();
};
