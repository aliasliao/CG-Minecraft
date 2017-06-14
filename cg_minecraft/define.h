#pragma once

enum class cub
{
	DIRT, GRASS, BRICK, STONE,
	BED_HEAD, BED_FEET,
	CRAFTING_TABLE, ROSE,
	LAST
};
enum class dir  // cube to human
{
	FRONT, BACK, LEFT, RIGHT, UP, BOTTOM
};
enum class cam
{
	front, back, left, right, zoomIn, zoomOut, reset
};