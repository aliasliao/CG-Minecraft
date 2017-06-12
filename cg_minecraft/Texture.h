#pragma once

#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <cstdlib>
#include "define.h"

/*
 * 0  -- default texture (if texture=0, color=gray; else=texture color)
 * 1  -- ground / grass
 * 16 -- transparent
 **/
class TexTure {
	void bindAndSet(const GLuint index, sf::Color color) {
		glBindTexture(GL_TEXTURE_2D, index);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		sf::Image image;
		image.create(16, 16, color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bindAndSet(const GLuint index, const std::string &imageName) {
		const std::string &fullName = "textures/blocks/" + imageName;
		
		glBindTexture(GL_TEXTURE_2D, index);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		sf::Image image;
		image.loadFromFile(fullName);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void initUnit() {
		GLuint textures[64];

		glActiveTexture(GL_TEXTURE0);
		this->bindAndSet(textures[0], sf::Color::Yellow);  // !!!for external only
		glActiveTexture(GL_TEXTURE1);
		this->bindAndSet(textures[1], "wool_colored_lime.png");
		glActiveTexture(GL_TEXTURE2);
		this->bindAndSet(textures[2], "dirt.png");
		glActiveTexture(GL_TEXTURE3);
		this->bindAndSet(textures[3], "grass_side.png");
		glActiveTexture(GL_TEXTURE4);
		this->bindAndSet(textures[4], "brick.png");
		glActiveTexture(GL_TEXTURE5);
		this->bindAndSet(textures[5], "cobblestone.png");
		glActiveTexture(GL_TEXTURE6);
		this->bindAndSet(textures[6], "crafting_table_front.png");
		glActiveTexture(GL_TEXTURE7);
		this->bindAndSet(textures[7], "crafting_table_side.png");
		glActiveTexture(GL_TEXTURE8);
		this->bindAndSet(textures[8], "crafting_table_top.png");
		glActiveTexture(GL_TEXTURE9);
		this->bindAndSet(textures[9], "bed_feet_end.png");
		glActiveTexture(GL_TEXTURE10);
		this->bindAndSet(textures[10], "bed_feet_side.png");
		glActiveTexture(GL_TEXTURE11);
		this->bindAndSet(textures[11], "bed_feet_top.png");
		glActiveTexture(GL_TEXTURE12);
		this->bindAndSet(textures[12], "bed_head_end.png");
		glActiveTexture(GL_TEXTURE13);
		this->bindAndSet(textures[13], "bed_head_side.png");
		glActiveTexture(GL_TEXTURE14);
		this->bindAndSet(textures[14], "bed_head_top.png");
		glActiveTexture(GL_TEXTURE15);
		this->bindAndSet(textures[15], "flower_rose.png");
		glActiveTexture(GL_TEXTURE16);
		this->bindAndSet(textures[16], sf::Color::Transparent);
	}

	std::map<Direction, int> genCube(int f, int b, int l, int r, int u, int bo) {
		std::map<Direction, int> tmp;
		tmp[FRONT] = f;
		tmp[BACK] = b;
		tmp[LEFT] = l;
		tmp[RIGHT] = r;
		tmp[UP] = u;
		tmp[BOTTOM] = bo;

		return tmp;
	}

	void initTex() {
		this->tex[DIRT			] = this->genCube( 2,  2,  2,  2,  2,  2);
		this->tex[GRASS			] = this->genCube( 3,  3,  3,  3,  1,  2);
		this->tex[BRICK			] = this->genCube( 4,  4,  4,  4,  4,  4);
		this->tex[STONE			] = this->genCube( 5,  5,  5,  5,  5,  5);
		this->tex[BED_HEAD		] = this->genCube(16, 12, 13, 13, 14, 14);
		this->tex[BED_FEET		] = this->genCube( 9, 16, 10, 10, 11, 11);
		this->tex[CRAFTING_TABLE] = this->genCube( 6,  6,  7,  7,  8,  8);
		this->tex[ROSE			] = this->genCube(15, 15, 15, 15, 16, 16);
	}

	// remember to free!
	void initIndexArray() {
		const int size = this->tex.size();
		this->indexArray = (GLuint *)malloc(size * sizeof(GLuint));
		for (int i = 0; i < size; i++) {
			this->indexArray[i] = i;
		}
	}

public:
	std::map<CubeType, std::map<Direction, int> > tex;
	GLuint *indexArray;

	TexTure() {
		this->initUnit();
		this->initTex();
		this->initIndexArray();
	}

	~TexTure() {
		free(this->indexArray);
	}
};

TexTure TEX = TexTure();