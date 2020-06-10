#pragma once

#include <array>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec2 pos;
	glm::vec2 tex;
	float shade = 0.5f;
};
