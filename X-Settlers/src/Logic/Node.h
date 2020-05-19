#pragma once

#include "Logic/Movable.h"

enum class AreaType { GRASS, MOUNTAIN, WATER, SAND };

class Node {
public:
	Movable* m_Movable;
	AreaType m_Type;
	bool walkable;
	uint8_t height;
};