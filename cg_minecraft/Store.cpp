#include "stdafx.h"
#include <cstdlib>
#include "Store.h"
#include "Texture.h"

Store::Store(bool needGround)
{
	if (needGround) {
		this->addGround();
	}
	this->hasGround = needGround;
	this->initBuffers();
}

Store::Store(const std::string fileName, bool hasGround)
{
	this->hasGround = hasGround;
	if (!hasGround) {  // a scene always need a ground
		this->addGround();
	}
	this->initBuffers();
}

// only for none ground
bool Store::addCube(
	const glm::ivec3 &position,
	const CubeType type  // type is CubeType other than GROUND
)
{
	if (this->getCubeIndex(position) <= 0) {
		std::cout << "position has been occupied or underground" << std::endl;
		return false;
	}

	// 6*2*3*3
	// vertices
	int orv[6 * 2 * 3 * 3] = {  // every point coordinate
		0,0,0, 0,0,1, 1,0,1,  1,0,1, 1,0,0, 0,0,0,
		0,1,0, 0,1,1, 1,1,1,  1,1,1, 1,1,0, 0,1,0,

		0,0,0, 0,0,1, 0,1,1,  0,1,1, 0,1,0, 0,0,0,
		1,0,0, 1,0,1, 1,1,1,  1,1,1, 1,1,0, 1,0,0,

		0,0,1, 1,0,1, 1,1,1,  1,1,1, 0,1,1, 0,0,1,
		1,0,0, 0,0,0, 0,1,0,  0,1,0, 1,1,0, 1,0,0
	};

	// normals
	int orn[6 * 3] = {  // 6 faces normal
		 0, -1,  0,
		 0,  1,  0,
		-1,  0,  0,
		 1,  0,  0,
		 0,  0,  1,
		 0,  0, -1
	};

	// texture coordinate
	int ort[6 * 2] = {  // every face texture coordinate
		0,0, 1,0, 1,1,  1,1, 0,1, 0,0
	};

	// texture index
	int ori[6];  // 6 faces texture index
	ori[0]= TEX.tex[type][BOTTOM];
	ori[1]= TEX.tex[type][UP];
	ori[2]= TEX.tex[type][LEFT];
	ori[3]= TEX.tex[type][RIGHT];
	ori[4]= TEX.tex[type][FRONT];
	ori[5]= TEX.tex[type][BACK];

	// 3*2*6, 3*2*6, 3*2*6, 2*6
	for (int i = 0; i < 6; i++) {
		unsigned int nextIndex = this->vertices.size();  // 3 index represent a face
		for (int j = 0; j < 6; j++) {
			int vi = 6 * 3 * i + 3 * j;  // index in orv
			this->vertices.push_back(position + glm::ivec3(orv[vi], orv[vi+1], orv[vi+2]));

			int ni = 3 * i;
			this->normals.push_back(glm::ivec3(orn[ni], orn[ni+1], orn[ni+2]));

			int ti = 2 * j;
			this->textures.push_back(glm::ivec3(ort[ti], ort[ti+1], ori[i]));
		}
		// generate 2 faces here
		this->elements.push_back(glm::uvec3(nextIndex  , nextIndex+1, nextIndex+2));
		this->elements.push_back(glm::uvec3(nextIndex+3, nextIndex+4, nextIndex+5));
	}

	this->upload();

	return true;
}

// only called for NONE ground
bool Store::removeCube(const glm::ivec3 & position)
{
	const int groundBias = this->hasGround ? 1 : 0;
	const int cubeIndex = this->getCubeIndex(position);

	if (cubeIndex <= 0) {
		std::cout << "cube not exits or try to remove ground" << std::endl;
		return false;
	}

	int beginBias = 2*3 * groundBias + (cubeIndex - 1) * 6*2*3;
	int endBias = 2*3 * groundBias + cubeIndex * 6*2*3;
	this->vertices.erase(this->vertices.begin() + beginBias, this->vertices.begin() + endBias);
	this->normals.erase(this->normals.begin() + beginBias, this->normals.begin() + endBias);
	this->textures.erase(this->textures.begin() + beginBias, this->textures.begin() + endBias);

	beginBias = 2 * groundBias + (cubeIndex - 1) * 6*2;
	endBias = 2 * groundBias + cubeIndex * 6*2;
	this->elements.erase(this->elements.begin() + beginBias, this->elements.begin() + endBias);

	this->upload();

	return true;
}

// need shader information
void Store::draw()
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);
	glDrawElements(GL_TRIANGLES, this->elements.size(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


//////////////////////////////////////////////////////////////////////////
// private functions

// only called for none ground
/*
 * <0 for not exits such cube
 * =0 for under ground
 * >0 for exits such cube, return cube index 1, 2, 3, ...
 */
int Store::getCubeIndex(const glm::ivec3 & position)
{
	const int groundBias = this->hasGround ? 1 : 0;
	const int interval = 6 * 2 * 3;

	if (position.y < 0) {  // under ground
		return 0;
	}

	for (int i = 0; i < this->vertices.size(); i++) {
		if ((i - groundBias) % interval == 0) {
			if (this->vertices[i] == position) {
				return (i - groundBias) / interval + 1;
			}
		}
	}

	return -1;  // not exits such cube
}

// only called for ground
void Store::addGround()
{
		this->vertices.push_back(glm::ivec3(-100, 0, -100));
		this->vertices.push_back(glm::ivec3(-100, 0, 100));
		this->vertices.push_back(glm::ivec3(100, 0, 100));
		this->vertices.push_back(glm::ivec3(100, 0, 100));
		this->vertices.push_back(glm::ivec3(100, 0, -100));
		this->vertices.push_back(glm::ivec3(-100, 0, -100));

		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));

		this->textures.push_back(glm::ivec3(0, 0, 0));
		this->textures.push_back(glm::ivec3(0, 1, 0));
		this->textures.push_back(glm::ivec3(1, 1, 0));
		this->textures.push_back(glm::ivec3(1, 1, 0));
		this->textures.push_back(glm::ivec3(1, 0, 0));
		this->textures.push_back(glm::ivec3(0, 0, 0));

		// !!!must be called first, ground should be the head of these vectors
		this->elements.push_back(glm::uvec3(0, 1, 2));
		this->elements.push_back(glm::uvec3(3, 4, 5));

		this->upload();
}

/*
 * in vertex shader:
 * vertex location=0, normal location=1, texture location=2
 **/
void Store::initBuffers()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vertexVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, 0);

	glGenBuffers(1, &this->normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, 0, 0);

	glGenBuffers(1, &this->textureVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_INT, GL_FALSE, 0, 0);

	glGenBuffers(1, &this->elementEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);

	// unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);  // break the existing vertex array object binding
}

void Store::upload()
{
	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->vertices.size() * sizeof(this->vertices[0]),
		this->vertices.data(),
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ARRAY_BUFFER, this->normalVbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->normals.size() * sizeof(this->normals[0]),
		this->normals.data(),
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ARRAY_BUFFER, this->textureVbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->textures.size() * sizeof(this->textures[0]),
		this->textures.data(),
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		this->elements.size() * sizeof(this->elements[0]),
		this->elements.data(),
		GL_STATIC_DRAW
	);

	// unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);  // break the existing vertex array object binding
}