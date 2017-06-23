#pragma once

enum class cub
{
	DIRT, GRASS, BRICK, STONE,
	BED_HEAD, BED_FEET,
	CRAFTING_TABLE, ROSE, FACE,
	PUMPKIN_ORIGIN_OFF, PUMPKIN_ORIGIN_ON,
	PLANK, FURNACE_OFF, FURNACE_ON,
	teapot, pumpkin, sphere, cylinder,
	LAST  // for iteration, cannot be removed!!!
};
enum class dir  // cube to human
{
	FRONT, BACK, LEFT, RIGHT, UP, BOTTOM
};
enum class cam
{
	front, back, left, right, up, down,
	zoomIn, zoomOut, reset
};

bool isExtModel(cub type);