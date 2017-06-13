#pragma once

#include <GL/glew.h>
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
class Texture
{
	std::vector<std::string> files;

	void fillFiles();
	void fillTex();
	std::map<dir, int> genCube(int f, int b, int l, int r, int u, int bo);

public:
	std::map<cub, std::map<dir, int> > tex;

	Texture();
	void uploadTextures();
};