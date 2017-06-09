#include "stdafx.h"
#include <cstdlib>
#include "Store.h"
#include "Texture.h"

Store::Store(bool isGround)
{
	this->isGround = isGround;
	this->initBuffers();
	if (isGround) {
		this->addGround();
	}
}

// only for none ground
bool Store::addCube(
	const glm::ivec3 &position,
	const CubeType type,  // type is CubeType other than GROUND
	const Direction zFacedirection
)
{
	if (this->isOccupied(position) || position.y<0) {
		std::cout << "position has been occupied or underground" << std::endl;
		return false;
	}

	// vertices
	int orv[6 * 4 * 3] = {  // every point coordinate
		0,0,0, 1,0,0, 1,0,1, 0,0,1,
		0,1,0, 1,1,0, 1,1,1, 0,1,1,
		0,0,0, 0,1,0, 0,1,1, 0,0,1,
		1,0,0, 1,1,0, 1,1,1, 1,0,1,
		0,0,1, 0,1,1, 1,1,1, 1,0,1,
		0,0,0, 0,1,0, 1,1,0, 1,0,0
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
	int ort[4 * 2] = {  // every face texture coordinate
		0,0, 0,1, 1,1, 1,0
	};

	// texture index
	int ori[6], tmp[6];  // 6 faces texture index
	ori[0]= tmp[0] = TEX.tex[type][BOTTOM];
	ori[1]= tmp[1] = TEX.tex[type][UP];
	ori[2]= tmp[2] = TEX.tex[type][LEFT];
	ori[3]= tmp[3] = TEX.tex[type][RIGHT];
	ori[4]= tmp[4] = TEX.tex[type][FRONT];
	ori[5]= tmp[5] = TEX.tex[type][BACK];
	switch (zFacedirection)
	{
		case FRONT:
			break;
		case BACK:
			ori[2] = tmp[3]; ori[3] = tmp[2]; ori[4] = tmp[5]; ori[5] = tmp[4];
			break;
		case LEFT:
			ori[2] = tmp[4]; ori[3] = tmp[5]; ori[4] = tmp[3]; ori[5] = tmp[2];
			break;
		case RIGHT:
			ori[2] = tmp[5]; ori[3] = tmp[4]; ori[4] = tmp[2]; ori[5] = tmp[3];
			break;
		default:
			std::cout << "invalid zFaceDirection" << std::endl; return false;
			break;
	}

	// 4*6, 4*6, 4*6, 1*6
	for (int i = 0; i < 6; i++) {  // bottom, up, left, right, front, back
		int nextIndex = this->vertices.size();  // 4 index represent a face
		for (int j = 0; j < 4; j++) {
			int vi = 4 * 3 * i + 3 * j;
			this->vertices.push_back(glm::ivec3(
				position.x + orv[vi],
				position.y + orv[vi+1],
				position.z + orv[vi+2]));

			int ni = 3 * j;
			this->normals.push_back(glm::ivec3(orn[ni], orn[ni+1], orn[ni+2]));

			int ti = 2 * j;
			this->textures.push_back(glm::ivec3(ort[ti], ort[ti+1], ori[i]));
		}
		this->elements.push_back(glm::ivec4(nextIndex, nextIndex + 1, nextIndex + 2, nextIndex + 3));
	}

	return true;
}

// only called for NONE ground
bool Store::removeCube(const glm::ivec3 & position)
{
	const int groundBias = this->isGround ? 1 : 0;
	const int cubeIndex = this->getCubeIndex(position);

	if (cubeIndex <= 0) {
		std::cout << "cube not exits or try to remove ground" << std::endl;
		return false;
	}

	int beginBias = 6 * groundBias + (cubeIndex - 1) * 24;
	int endBias = 6 * groundBias + cubeIndex * 24;
	this->vertices.erase(this->vertices.begin() + beginBias, this->vertices.begin() + endBias);
	this->normals.erase(this->normals.begin() + beginBias, this->normals.begin() + endBias);
	this->textures.erase(this->textures.begin() + beginBias, this->textures.begin() + endBias);

	beginBias = 1 * groundBias + (cubeIndex - 1) * 6;
	endBias = 1 * groundBias + cubeIndex * 6;
	this->elements.erase(this->elements.begin() + beginBias, this->elements.begin() + endBias);

	return true;
}

// need shader information
void Store::draw()
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);
	glDrawElements()
}


// private functions

// only called for none ground
// means it is occupied by cube
bool Store::isOccupied(const glm::ivec3 & position)
{
	if (this->getCubeIndex(position) <= 0) {
		return false;
	}
	else {
		return true;
	}
}

// only called for none ground
int Store::getCubeIndex(const glm::ivec3 & position)
{
	const int groundBias = this->isGround ? 1 : 0;

	if (position.y < 0) {  // under ground
		return 0;
	}

	for (int i = 0; i < this->vertices.size(); i++) {
		if ((i - groundBias) % 24 == 0) {
			if (this->vertices[i] == position) {
				return (i - groundBias) / 24 + 1;
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
		this->vertices.push_back(glm::ivec3(100, 0, -100));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->normals.push_back(glm::ivec3(0, 1, 0));
		this->textures.push_back(glm::ivec3(0, 1, 0));
		this->textures.push_back(glm::ivec3(1, 1, 0));
		this->textures.push_back(glm::ivec3(1, 0, 0));
		this->textures.push_back(glm::ivec3(0, 0, 0));
		this->elements.push_back(glm::ivec4(0, 1, 2, 3));  // !!!must be called first
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