#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <engine.h>

#include "logic/playfield.h"

enum class PartType {
	EoF = 0, MapInfo = 1, PlayerInfo = 2, TeamInfo = 3,
	Preview = 4, Area = 6, Settlers = 7, Buildings = 8,
	Resources = 9, QuestText = 11, QuestTip = 12, Count = 12
};

enum class MapMode {
	// TODO: set the correct values being used in the map files
	Conflict, Economy, Free
};

struct PartHeader
{
	uint32_t type;
	uint32_t length;
	uint32_t offset;
};

struct MapInfo
{
	std::string path;
	uint32_t size;
	uint8_t player_count;
	uint8_t resource_preset;
	bool is_campaign;
	MapMode mode;
	std::vector<PartHeader> parts;
	// TODO: add preview image to pre-loaded map data
};

struct MapArea
{
	std::vector<uint8_t> heights;
	std::vector<uint8_t> types;
	std::vector<uint8_t> objects;
	std::vector<uint8_t> player_claims;
	std::vector<uint8_t> accessibles;
	std::vector<uint8_t> resources;
};

class MapLoader
{
public:
	static void init();
	static void shutdown();

	static std::vector<MapInfo> get_map_list();

	static void load_map_area(const uint32_t map_index, MapArea& map);
private:
	static void index_map_file(const std::string& path);
	static void decrypt_partition(std::vector<char>& map_file, const PartHeader& part_header);
};
