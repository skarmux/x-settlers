#pragma once

#include "movable.h"

enum class AreaType
{ 
	Grass, Mountain, Water, Sand
};

struct Node {
	Movable* movable;
	AreaType type;
	bool walkable;
	uint8_t height;
};