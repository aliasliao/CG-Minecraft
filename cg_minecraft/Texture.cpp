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
		/*17*/"custom_face1.png",
		/*18*/"custom_face2.png",
		/*19*/"custom_face3.png",
		/*20*/"custom_face4.png",
		/*21*/"pumpkin_face_off.png",
		/*22*/"pumpkin_face_on.png",
		/*23*/"pumpkin_side.png",
		/*24*/"pumpkin_top.png",
		/*25*/"planks_birch.png",
		/*26*/"furnace_front_off.png",
		/*27*/"furnace_front_on.png",
		/*28*/"furnace_side.png",
		/*29*/"furnace_top.png",
	};
}

void Texture::fillTex()  // front, back, left, right, up, bottom
{
	this->tex[cub::DIRT				 ] = this->genCube( 3,  3,  3,  3,  3,  3);
	this->tex[cub::GRASS		 	 ] = this->genCube( 4,  4,  4,  4,  2,  3);
	this->tex[cub::BRICK		 	 ] = this->genCube( 5,  5,  5,  5,  5,  5);
	this->tex[cub::STONE		 	 ] = this->genCube( 6,  6,  6,  6,  6,  6);
	this->tex[cub::BED_HEAD		 	 ] = this->genCube( 1, 13, 14, 14, 15,  1);
	this->tex[cub::BED_FEET		 	 ] = this->genCube(10,  1, 11, 11, 12,  1);
	this->tex[cub::CRAFTING_TABLE	 ] = this->genCube( 7,  7,  8,  8,  9,  9);
	this->tex[cub::ROSE			 	 ] = this->genCube(16, 16, 16, 16,  1,  1);
	this->tex[cub::FACE			 	 ] = this->genCube(17, 20, 18, 18, 19,  1);
	this->tex[cub::PUMPKIN_ORIGIN_OFF] = this->genCube(21, 23, 23, 23, 24, 23);
	this->tex[cub::PUMPKIN_ORIGIN_ON ] = this->genCube(22, 23, 23, 23, 24, 23);
	this->tex[cub::PLANK			 ] = this->genCube(25, 25, 25, 25, 25, 25);
	this->tex[cub::FURNACE_OFF		 ] = this->genCube(26, 28, 28, 28, 29, 29);
	this->tex[cub::FURNACE_ON		 ] = this->genCube(27, 28, 28, 28, 29, 29);
}

void Texture::fillCubeName()
{
	this->cubName[cub::DIRT				 ] = "dirt";
	this->cubName[cub::GRASS		 	 ] = "grass";
	this->cubName[cub::BRICK		 	 ] = "brick";
	this->cubName[cub::STONE		 	 ] = "stone";
	this->cubName[cub::BED_HEAD		 	 ] = "bed (from head)";
	this->cubName[cub::BED_FEET		 	 ] = "bed (from feet)";
	this->cubName[cub::CRAFTING_TABLE	 ] = "crafting table";
	this->cubName[cub::ROSE			 	 ] = "rose";
	this->cubName[cub::FACE			 	 ] = "face";
	this->cubName[cub::PUMPKIN_ORIGIN_OFF] = "origin pumpkin (off)";
	this->cubName[cub::PUMPKIN_ORIGIN_ON ] = "origin pumpkin (on)";
	this->cubName[cub::PLANK			 ] = "plank";
	this->cubName[cub::FURNACE_OFF		 ] = "furnace (off)";
	this->cubName[cub::FURNACE_ON		 ] = "furnace (on)";

	this->cubName[cub::teapot			 ] = "teapot";
	this->cubName[cub::pumpkin			 ] = "pumpkin";
	this->cubName[cub::sphere			 ] = "sphere";
	this->cubName[cub::cylinder			 ] = "cylinder";
}