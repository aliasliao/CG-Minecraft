#include "stdafx.h"
#include <string>
#include <cstdlib>
#include "Store.h"
#include "Texture.h"

Store::Store(bool isGround)
{
	this->isGround = isGround;
	this->initBuffers();
	if (isGround) {
		this->addGround();  // !contains upload here!
	}
}

Store::Store(const std::string &fileName, bool isExternal)
{
	this->isGround = false;
	this->initBuffers();
	if (isExternal) {
		this->loadExternal(fileName, glm::ivec3(0, 0, 0));
	}
	else {
		this->loadState(fileName);
	}
}

// only for none ground
bool Store::addCube(
	const glm::ivec3 &position,
	const cub type,  // type is CubeType other than GROUND
	Texture &TEX
)
{
	if (this->getCubeIndex(position) >= 0) {
		std::cout << "position has been occupied or underground" << std::endl;
		return false;
	}

	if (isExtModel(type)) {  // for external store only
		return this->addExternalModel(position, type, TEX);
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
		1,1, 0,1, 0,0,  0,0, 1,0, 1,1
	};

	// texture index
	int ori[6];  // 6 faces texture index
	ori[0]= TEX.tex[type][dir::BOTTOM];
	ori[1]= TEX.tex[type][dir::UP];
	ori[2]= TEX.tex[type][dir::LEFT];
	ori[3]= TEX.tex[type][dir::RIGHT];
	ori[4]= TEX.tex[type][dir::FRONT];
	ori[5]= TEX.tex[type][dir::BACK];

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
		this->elements.push_back(glm::uvec3(nextIndex,   nextIndex+1, nextIndex+2));
		this->elements.push_back(glm::uvec3(nextIndex+3, nextIndex+4, nextIndex+5));
	}

	this->upload();

	return true;
}

// only for external store only, add/remove is forbidden
bool Store::addExternalModel(const glm::ivec3 & position, const cub type, Texture & TEX)
{
	std::string fname = TEX.cubName[type] + ".obj";
	this->loadExternal(fname, position);

	return false;
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

void Store::drawArrays()
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Store::getBufferSize(GLenum target, GLint &n)
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementEbo);
	glGetBufferParameteriv(target, GL_BUFFER_SIZE, &n);
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
	const int groundBias = this->isGround ? 1 : 0;
	const int interval = 6 * 2 * 3;

	if (position.y < 0) {  // under ground
		return 0;
	}

	for (unsigned int i = 0; i < this->vertices.size(); i++) {
		if ((i - groundBias) % interval == 0) {
			if (this->vertices[i] == glm::vec3(position)) {
				return (i - groundBias) / interval + 1;
			}
		}
	}

	return -1;  // not exits such cube
}

// only called for ground
bool Store::addGround()
{
	if (this->vertices.size() > 0) {
		std::cout << "fatal error: add ground to not empty store!" << std::endl;
		return false;
	}

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

	this->textures.push_back(glm::ivec3(0, 0, 2));
	this->textures.push_back(glm::ivec3(0, 1, 2));
	this->textures.push_back(glm::ivec3(1, 1, 2));
	this->textures.push_back(glm::ivec3(1, 1, 2));
	this->textures.push_back(glm::ivec3(1, 0, 2));
	this->textures.push_back(glm::ivec3(0, 0, 2));

	// !!!must be called first, ground should be the head of these vectors
	this->elements.push_back(glm::uvec3(0, 1, 2));
	this->elements.push_back(glm::uvec3(3, 4, 5));

	this->upload();

	return true;
}

/*
 * in vertex shader:
 * vertex location=0,
 * normal location=1,
 * texture location=2
 **/
void Store::initBuffers()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vertexVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &this->normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &this->textureVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

bool Store::loadState(const std::string & fileName)
{
	const std::string fullName = "models/" + fileName;

	FILE *fp = fopen(fullName.c_str(), "r");
	if (fp == NULL) {
		std::cout << "can not open file " << fileName << std::endl;
		return false;
	}

	char lineHeader[128];
	float vx, vy, vz;
	float nx, ny, nz;
	float tx, ty, tt;
	unsigned int ev1, ev2, ev3, tmp;

	while (true) {
		int size = fscanf(fp, "%s", lineHeader);
		if (size == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			fscanf(fp, "%f %f %fn", &vx, &vy, &vz);
			this->vertices.push_back(glm::vec3(vx, vy, vz));
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			fscanf(fp, "%f %f %fn", &nx, &ny, &nz);
			this->normals.push_back(glm::vec3(nx, ny, nz));
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			fscanf(fp, "%f %f %fn", &tx, &ty, &tt);
			this->textures.push_back(glm::vec3(tx, ty, tt));
		}
		else if (strcmp(lineHeader, "f") == 0) {
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%dn", &ev1, &tmp, &tmp, &ev2, &tmp, &tmp, &ev3, &tmp, &tmp);
			this->elements.push_back(glm::uvec3(ev1-1, ev2-1, ev3-1));
		}
		else {
			// ignore blank line and # comment line
		}
	}

	this->upload();

	fclose(fp);
	return true;
}

// only have v, vn, f
// TODO: generate normals
bool Store::loadExternal(const std::string & fileName, const glm::ivec3 & position)
{
	const std::string fullName = "models/" + fileName;

	FILE *fp = fopen(fullName.c_str(), "r");
	if (fp == NULL) {
		std::cout << "can not open external file " << fileName << std::endl;
		return false;
	}

	char lineHeader[128];
	float vx, vy, vz;
	float nx, ny, nz;
	int ev1, ev2, ev3, tmp;

	bool isKnown = false;
	bool has3;

	while (true) {
		int size = fscanf(fp, "%s", lineHeader);
		if (size == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			fscanf(fp, "%f %f %fn", &vx, &vy, &vz);
			this->vertices.push_back(glm::vec3(vx+position.x, vy+position.y, vz+position.z));
			this->textures.push_back(glm::vec3(0, 0, 0));
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			fscanf(fp, "%f %f %fn", &nx, &ny, &nz);
			this->normals.push_back(glm::vec3(nx, ny, nz));
		}
		else if (strcmp(lineHeader, "f") == 0) {
			if (!isKnown) {
				char tmpStr[32];
				long savePos = ftell(fp);
				//fscanf(fp, "%d", &tmp);
				fscanf(fp, "%s", tmpStr);
				if (tmpStr[2] == '/')
					has3 = false;
				else
					has3 = true;
				fseek(fp, savePos, SEEK_SET);
				isKnown = true;
			}

			if (!has3)
				fscanf(fp, "%d//%d %d//%d %d//%dn", &ev1, &tmp, &ev2, &tmp, &ev3, &tmp);
			else
				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%dn", &ev1, &tmp, &tmp, &ev2, &tmp, &tmp, &ev3, &tmp, &tmp);
			this->elements.push_back(glm::uvec3(ev1-1, ev2-1, ev3-1));
		}
		else {
			// ignore blank line and # comment line
		}
	}

	this->upload();

	fclose(fp);
	return true;
}

bool Store::saveState(const std::string & fileName)
{
	const std::string fullName = "models/" + fileName;

	FILE *fp = fopen(fullName.c_str(), "w");
	if (fp == NULL) {
		std::cout << "can not write to file " << fileName << std::endl;
		return false;
	}

	fprintf(fp, "# vertices\n");
	for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
		fprintf(fp, "v %f %f %f\n", it->x, it->y, it->z);
	}

	fprintf(fp, "# normals\n");
	for (auto it = this->normals.begin(); it != this->normals.end(); it++) {
		fprintf(fp, "vn %f %f %f\n", it->x, it->y, it->z);
	}

	fprintf(fp, "# textures\n");
	for (auto it = this->textures.begin(); it != this->textures.end(); it++) {
		fprintf(fp, "vt %f %f %f\n", it->x, it->y, it->z);
	}

	fprintf(fp, "# faces\n");
	for (auto it = this->elements.begin(); it != this->elements.end(); it++) {
		glm::uvec3 iit = *it + glm::uvec3(1,1,1);
		fprintf(fp, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", iit.x, iit.x, iit.x, iit.y, iit.y, iit.y, iit.z, iit.z, iit.z);
	}

	fclose(fp);
	return true;
}