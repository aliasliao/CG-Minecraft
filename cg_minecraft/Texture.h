#pragma once

#include "GL/glew.h"
#include <map>
#include "define.h"

class TexTure {
public:
	std::map<CubeType, std::map<Direction, int>> tex;

	TexTure();
};

TexTure TEX = TexTure();