#include <engine.h>
#include <core/entry_point.h>

#include "layer/terrain_layer.h"

const uint32_t WIDTH = 1280, HEIGHT = 720;

class XSettlers : public Application
{
public:
	XSettlers() : 
		Application("X-Settlers", WIDTH, HEIGHT)
	{
		MapLoader::init();

		std::vector<MapInfo>& maps = MapLoader::map_list();
		for (MapInfo map_info : maps) {
			LOGIC_INFO("loaded map {0}:\n\twidth: {1}, is campaign: {2}, players: {3}, resources: {4}",
				map_info.path,
				map_info.size,
				map_info.is_campaign,
				map_info.player_count,
				map_info.resource_preset
			);
		}

		uint32_t selected_map = 0;

		MapInfo map_metadata = maps[selected_map]; // select first map from selection

		MapNode* const nodes = new MapNode[(uint64_t)map_metadata.size * (uint64_t)map_metadata.size];

		MapLoader::load_map_area(selected_map, nodes);

		push_layer(new TerrainLayer(WIDTH, HEIGHT, nodes, map_metadata.size));

		delete[] nodes;
	}

	~XSettlers() {}
};

Application* create_application()
{
	return new XSettlers();
}
