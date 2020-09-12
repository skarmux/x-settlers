#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>

enum class PartType {
	EoF = 0, MapInfo = 1, PlayerInfo = 2, TeamInfo = 3,
	Preview = 4, Area = 6, Settlers = 7, Buildings = 8,
	Resources = 9, QuestText = 11, QuestTip = 12, Count = 12
};

struct PartHeader
{
	uint32_t type;
	uint32_t data_size;
	uint32_t data_offset;
};

struct MapNode
{
	uint8_t height;
	uint8_t type;
	uint8_t object;
	uint8_t player_claim;
	uint8_t accessible;
	uint8_t resource;
};

struct MapInfo
{
	std::string path;
	uint32_t size;
	uint32_t player_count;
	uint32_t resource_preset;
	bool is_campaign;
	std::vector<PartHeader> parts;
};

class MapLoader
{
public:
	static void init();
	static void shutdown();

	static std::vector<MapInfo> map_list();

	static void load_map_area(uint32_t map_index, MapNode* area);
private:
	static void index_map_file(const std::string& path);
	static void decrypt_partition(std::vector<char>& map_file, const PartHeader& part_header);
};
