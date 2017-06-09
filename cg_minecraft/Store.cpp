#include "stdafx.h"
#include <cstdlib>
#include "Store.h"
#include "Texture.h"

bool Store::addCube(const glm::ivec3 &position,
	const CubeType type,
	const Direction zFacedirection)
{
	if (this->isOccupied(position) || position.y<0) {
		std::cout << "position has been occupied or underground" << std::endl;
		return false;
	}

	if (type == GROUND) {
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

		return true;
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

	// elements

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

bool Store::removeCube(const glm::ivec3 & position)
{
	const int cubeIndex = this->getCubeIndex(position);

	if (cubeIndex <= 0) {
		std::cout << "cube not exits or try to remove ground" << std::endl;
		return false;
	}

	return true;
}