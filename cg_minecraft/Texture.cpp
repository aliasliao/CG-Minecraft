#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
{
	this->fillTex();
	this->fillFiles();
	this->fillCubeName();
}

void Texture::uploadTextures()
{
	GLuint texture;
	GLsizei width = 16;
	GLsizei height = 16;
	GLsizei layerCount = this->files.size();
	GLsizei mipLevelCount = 1;
	sf::Image image;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);

	for (int i = 0; i < layerCount; i++) {
		image.loadFromFile("textures/blocks/" + this->files[i]);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

//////////////////////////////////////////////////////////////////////////

std::map<dir, int> Texture::genCube(int f, int b, int l, int r, int u, int bo)
{
	std::map<dir, int> tmp;
	tmp[dir::FRONT	] = f;
	tmp[dir::BACK	] = b;
	tmp[dir::LEFT	] = l;
	tmp[dir::RIGHT	] = r;
	tmp[dir::UP		] = u;
	tmp[dir::BOTTOM	] = bo;

	return tmp;
}

void Texture::fillFiles()
{
	this->files = {
		/* 0*/"custom_lightgray.png",
		/* 1*/"custom_transparent.png",
		/* 2*/"wool_colored_lime.png",
		/* 3*/"dirt.png",
		/* 4*/"grass_side.png",
		/* 5*/"brick.png",
		/* 6*/"cobblestone.png",
		/* 7*/"crafting_table_front.png",
		/* 8*/"crafting_table_side.png",
		/* 9*/"crafting_table_top.png",
		/*10*/"bed_feet_end.png",
		/*11*/"bed_feet_side.png",
		/*12*/"bed_feet_top.png",
		/*13*/"bed_head_end.png",
		/*14*/"bed_head_side.png",
		/*15*/"bed_head_top.png",
		/*16*/"flower_rose.png",
	};
}

void Texture::fillTex()  // front, back, left, right, up, bottom
{
	this->tex[cub::DIRT				] = this->genCube( 3,  3,  3,  3,  3,  3);
	this->tex[cub::GRASS		 	] = this->genCube( 4,  4,  4,  4,  2,  3);
	this->tex[cub::BRICK		 	] = this->genCube( 5,  5,  5,  5,  5,  5);
	this->tex[cub::STONE		 	] = this->genCube( 6,  6,  6,  6,  6,  6);
	this->tex[cub::BED_HEAD		 	] = this->genCube( 1, 13, 14, 14, 15,  1);
	this->tex[cub::BED_FEET		 	] = this->genCube(10,  1, 11, 11, 12,  1);
	this->tex[cub::CRAFTING_TABLE	] = this->genCube( 7,  7,  8,  8,  9,  9);
	this->tex[cub::ROSE			 	] = this->genCube(16, 16, 16, 16,  1,  1);
}

void Texture::fillCubeName()
{
	this->cubName[cub::DIRT				] = "dirt";
	this->cubName[cub::GRASS		 	] = "grass";
	this->cubName[cub::BRICK		 	] = "brick";
	this->cubName[cub::STONE		 	] = "stone";
	this->cubName[cub::BED_HEAD		 	] = "bed(from head)";
	this->cubName[cub::BED_FEET		 	] = "bed(from feet)";
	this->cubName[cub::CRAFTING_TABLE	] = "crafting table";
	this->cubName[cub::ROSE			 	] = "rose";
}