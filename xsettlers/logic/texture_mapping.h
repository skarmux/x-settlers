#pragma once

#include <glm/vec2.hpp>

#include <unordered_map>

enum class GroundType
{
	Sea = 7,
	Grass = 16,
	Desert = 20,
	Rock = 32,
	Beach = 48,
	Desert = 64,
	Desert = 65,
	Swamp = 80,
	Ice = 128,
	Mud = 144,
	COUNT = 145
};

/*
Hexagonal (outer, [ (inner,[tex_id(s)]), ... ])
16, [ (96,[52,53]), ... ]
(tex_id, outer_type, inner_type)
*/

const uint8_t type_count    = 144;
const uint8_t variant_count = 2;

std::unordered_map<uint8_t, uint8_t[2]> map[static_cast<uint8_t>(GroundType::COUNT)];

uint8_t tex_map[type_count][type_count][variant_count] = {
	{},
	{},
	{} };



// 144 different types

char TEX_IDS[] = {
	 0,
	 1,
	 3,
	 4,
	 5,
	 6,
	10, //  7 Sea
	 8,
	 9,
	10,
	11,
	12,
	13,
	14,
	15,
	 0, // 16 Grass
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	21, // 32 Rock
	33

};