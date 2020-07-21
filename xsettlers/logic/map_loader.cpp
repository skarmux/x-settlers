#include "logic/map_loader.h"
#include "platform/filesystem.h"

#include <filesystem>
#include <engine.h>

static const std::string MAPS_PATH = "assets/maps/s3";

struct MapLoaderData
{
	std::vector<MapInfo> map_infos;
};

static MapLoaderData s_data;

void MapLoader::init()
{
	s_data.map_infos = std::vector<MapInfo>(0);

	for (const auto& entry : std::filesystem::directory_iterator(MAPS_PATH))
	{
		LOGIC_INFO("loading map file: {0}", entry.path().string());
		index_map_file(entry.path().string());
	}
}

void MapLoader::shutdown()
{
	s_data.map_infos.clear();
}

void MapLoader::index_map_file(const std::string& path)
{
	std::vector<uint8_t> map_file = read_file(path);

	MapInfo map_info{};
	map_info.path = path;
	map_info.parts = std::vector<PartHeader>((int)PartType::Count);
	// TODO: Not necessary to instantiate PartHeader vector with maximum size
	// as many part types aren't present with plenty of maps

	uint8_t* buffer = map_file.data() + 8; // first partition starts at 8th byte

	PartHeader part_header{};

	do {
		// set partition header
		memcpy(&part_header.type, buffer, sizeof(uint32_t));
		memcpy(&part_header.data_size, buffer + 4, sizeof(uint32_t));
		buffer += 8;
		part_header.data_offset = buffer - (uint8_t*)map_file.data();

		// decrypt partition header
		part_header.type   &= 0x0000FFFF;
		part_header.data_size += 0xFFFFFFF8;

		LOGIC_INFO("\tpartition : type {0}, length {1}, offset {2}",
			part_header.type, part_header.data_size, part_header.data_offset );
		
		if (part_header.type > 0 && part_header.type <= static_cast<uint32_t>(PartType::Count) )
			map_info.parts[part_header.type] = part_header;

		switch (part_header.type)
		{
		case static_cast<uint32_t>(PartType::MapInfo):

			decrypt_partition(map_file, part_header);
			memcpy(&map_info.is_campaign, buffer, sizeof(uint32_t));
			memcpy(&map_info.player_count, buffer + 4, sizeof(uint32_t));
			memcpy(&map_info.resource_preset, buffer + 8, sizeof(uint32_t));
			break;

		case static_cast<uint32_t>(PartType::Area):

			decrypt_partition(map_file, part_header);
			memcpy(&map_info.size, buffer, sizeof(uint32_t));
			break;
		}

		buffer += part_header.data_size;

	} while (part_header.type != 0);

	map_file.clear();

	s_data.map_infos.push_back(map_info);
}

std::vector<MapInfo> MapLoader::get_map_list()
{
	// TODO: Singleton shouldn't have any getter
	return s_data.map_infos;
}

void MapLoader::decrypt_partition(std::vector<uint8_t>& map_file, const PartHeader& part_header)
{
	uint8_t* buffer = map_file.data();
	buffer += part_header.data_offset;

	char key = part_header.type & 0xFF;

	for (uint32_t i = 0; i < part_header.data_size; i++) {
		/*char byt = *buffer ^ key;
		*buffer = byt;
		buffer++;
		key = ((key << 1) & 0xFF) ^ byt;*/
		*buffer ^= key;	
		key = ((key << 1) & 0xFF) ^ *buffer;
		buffer++;
	}
}

void MapLoader::load_map_area(uint32_t map_index, MapNode* area)
{	
	const MapInfo& map_info = s_data.map_infos[map_index];
	const PartHeader& area_part_header = map_info.parts[static_cast<uint32_t>(PartType::Area)];

	std::vector<uint8_t> map_file = read_file(map_info.path);

	decrypt_partition(map_file, area_part_header);

	uint8_t* buffer = map_file.data();
	buffer += area_part_header.data_offset;
	buffer += 4; // skip size value (4 Byte) that is already known

	memcpy(area, buffer, (uint64_t)map_info.size * (uint64_t)map_info.size * sizeof(MapNode));

	map_file.clear();
}
