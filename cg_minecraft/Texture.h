#pragma once

#include "GL/glew.h"
#include <map>
#include "define.h"

/*
 * 0 -- no texture (if texture=0, color=gray; else=texture color)
 * 1 -- ground / grass
 **/
class TexTure {
public:
	std::map<CubeType, std::map<Direction, int>> tex;

	TexTure() {}
};

TexTure TEX = TexTure();