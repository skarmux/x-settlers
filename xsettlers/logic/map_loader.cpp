#include <filesystem>

#include "logic/map_loader.h"
#include "platform/filesystem.h"

struct MapLoaderData
{
	// TODO: figuring out the proper const std::string definition approach
	inline static const std::string MAPS_PATH = "assets/maps/s3";

	std::vector<MapInfo> map_infos;
};

static MapLoaderData s_data;

void MapLoader::init()
{
	// fetch MapData for each map file within 'assets/maps/' folder
	for (const auto& entry : std::filesystem::directory_iterator(s_data.MAPS_PATH))
	{
		LOGIC_INFO("loading map file: {0}", entry.path().string());
		index_map_file(entry.path().string());
	}
}

void MapLoader::shutdown()
{
	s_data.map_infos.clear();
}

std::vector<MapInfo> MapLoader::get_map_list()
{
	return s_data.map_infos;
}

void MapLoader::index_map_file(const std::string& path)
{
	std::vector<char> map_file = read_file(path);

	MapInfo map_info;
	map_info.path = path;
	map_info.parts = std::vector<PartHeader>((int)PartType::Count);

	PartHeader part_header;

	char* buffer = map_file.data() + 8; // first partition starts at 8th byte

	do {
		// set partition header
		memcpy(&part_header.type, buffer, sizeof(part_header.type));
		memcpy(&part_header.length, buffer + 4, sizeof(part_header.length));
		buffer += 8;
		part_header.offset = buffer - map_file.data();

		// decrypt partition header
		//part_header.type << 16;                   // bit shift
		part_header.type = part_header.type & 0x0000FFFF; // masking
		part_header.length = part_header.length + 0xFFFFFFF8; // masking

		LOGIC_INFO("\tpartition : type {0}, length {1}, offset {2}",
			part_header.type, part_header.length, part_header.offset
		);
		
		if (part_header.type > 0 && part_header.type <= (int)PartType::Count)
		{
			//map_info.parts.push_back(part_header);
			map_info.parts[part_header.type] = part_header;
		}

		// we need following information to be pre-loaded for the maps overview
		// [MapInfo] >> player_count, resource_preset, map_mode
		// [Area] >> size
		switch (part_header.type)
		{
		case (int)PartType::MapInfo:
			decrypt_partition(map_file, part_header);
			memcpy(&map_info.is_campaign, buffer, sizeof(int));
			memcpy(&map_info.player_count, buffer + 4, sizeof(int));
			memcpy(&map_info.resource_preset, buffer + 8, sizeof(int));
			break;
		case (int)PartType::Area:
			decrypt_partition(map_file, part_header);
			memcpy(&map_info.size, buffer, sizeof(int));
			break;
		}

		//part_header.offset += (8 + part_header.length);
		buffer += part_header.length;

	} while (part_header.type != 0);

	map_file.clear();

	s_data.map_infos.push_back(map_info);
}

void MapLoader::decrypt_partition(std::vector<char>& map_file, const PartHeader& part_header)
{
	unsigned char key = (uint32_t)part_header.type & 0xFF;
	char* buffer = map_file.data();
	buffer += part_header.offset;

	for (int i = 0; i < part_header.length; i++) {
		unsigned char byt = *buffer ^ key;
		*buffer = byt;
		buffer++;
		key = ((key << 1) & 0xFF) ^ byt;
	}
}

void MapLoader::load_map_area(const uint32_t map_index, MapArea& map)
{	
	const MapInfo& map_info = s_data.map_infos[map_index];
	const PartHeader& area_part_header = map_info.parts[(int)PartType::Area];
	const uint32_t node_count = map_info.size * map_info.size;

	map.accessibles.resize(node_count);
	map.heights.resize(node_count);
	map.objects.resize(node_count);
	map.player_claims.resize(node_count);
	map.resources.resize(node_count);
	map.types.resize(node_count);

	std::vector<char> map_file = read_file(map_info.path);

	decrypt_partition(map_file, area_part_header);

	uint8_t* buffer = (uint8_t*)map_file.data();
	buffer += area_part_header.offset;
	buffer += 4; // skip size value (4 Byte)
	
	for (uint32_t i = 0; i < node_count; i++)
	{
		map.heights[i]       = *buffer++;
		map.types[i]         = *buffer++;
		map.objects[i]       = *buffer++;
		map.player_claims[i] = *buffer++;
		map.accessibles[i]   = *buffer++;
		map.resources[i]     = *buffer++;
	}

	map_file.clear();
}
