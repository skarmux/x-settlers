#pragma once

#include "logic/movable.h"

enum class AreaType { GRASS, MOUNTAIN, WATER, SAND };

struct Node {
	Movable* movable;
	AreaType type;
	bool walkable;
	uint8_t height;
};