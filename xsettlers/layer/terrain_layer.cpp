#include "layer/terrain_layer.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

inline static glm::vec2 gridpos_to_world(uint32_t x, uint32_t y, uint32_t width)
{
	constexpr uint32_t GRID_X_STEP = 16;
	constexpr uint32_t GRID_Y_STEP = 9;

	return glm::vec2(x * GRID_X_STEP, (width - y) * GRID_Y_STEP);
}

static uint32_t tex_id_from_types(uint8_t type_a, uint8_t type_b, uint8_t type_c)
{
	glm::vec2 tex_coords[] = { {0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f} };

	/* Soil = 1, Bricks = 2, Sand = 3, Sea = 7, Grass = 16,
	Rock = 32, SandTrace = 35, DrySandTrace = 36, Beach = 48,
	Desert = 64, Swamp = 80, Ice = 128, Mud = 144, DarkSoil = 217,
	Cement = 230 */

	//switch (m_map_area[(width * (row - padding)) + col - padding].type)
	//{
	//case 0:
	//case 1: s_data.rhomb_vertex_buffer_ptr->tex_index = 1; break; // grass
	//case 2:
	//case 3: s_data.rhomb_vertex_buffer_ptr->tex_index = 3; break; // sand
	//case 7: s_data.rhomb_vertex_buffer_ptr->tex_index = 4; break; // water
	//case 10:
	//case 16: s_data.rhomb_vertex_buffer_ptr->tex_index = 1; break; // grass
	//case 17: s_data.rhomb_vertex_buffer_ptr->tex_index = 4; break; // water
	//case 21:
	//case 32: s_data.rhomb_vertex_buffer_ptr->tex_index = 2; break; // rock
	//case 35:
	//case 36:
	//case 48:
	//case 64: s_data.rhomb_vertex_buffer_ptr->tex_index = 3; break; // sand
	//default: s_data.rhomb_vertex_buffer_ptr->tex_index = 1;
	//}

	return 0;
}

void TerrainLayer::on_attach()
{
	MapLoader::init();

	TextureAtlas atlas;
	std::vector<uint32_t> texture_ids;
	{
		// fetch MapData for each map file within 'assets/maps/' folder
		const std::string texture_folder = "assets/textures/s3/terrain/";
		for (uint32_t i = 0; i < 235; i++)
		{
			uint32_t tmp_id;
			atlas.add_texture(texture_folder + std::to_string(i) + ".png", tmp_id);
			texture_ids.push_back(tmp_id);
		}

		// how to get from texture space to texture coordinate range?
		// available information x/y offset and width/height plus atlas width/height
		std::vector<TextureAtlas::Space> texture_spaces(texture_ids.size());
		for (uint32_t i = 0; i < texture_ids.size(); i++)
			texture_spaces[i] = atlas.get_texture_space(texture_ids[i]);
	}

	m_tex_terrain.emplace_back(Texture2D::create("assets/textures/free/0.png")); // grass
	m_tex_terrain.emplace_back(Texture2D::create("assets/textures/free/1.png")); // sand
	m_tex_terrain.emplace_back(Texture2D::create("assets/textures/free/2.png")); // rock
	m_tex_terrain.emplace_back(Texture2D::create("assets/textures/free/3.png")); // water

	// LOAD MAP

	

	std::vector<MapInfo>& maps = MapLoader::get_map_list();
	for (MapInfo map_info : maps) {
		LOGIC_INFO("loaded map {0}:\n\twidth: {1}, is campaign: {2}, players: {3}, resources: {4}",
			map_info.path,
			map_info.size,
			map_info.is_campaign,
			map_info.player_count,
			map_info.resource_preset
		);
	}

	// select map from selection
	m_map_info = maps[0];

	m_map_area = new MapNode[(uint64_t)m_map_info.size * (uint64_t)m_map_info.size];
	MapLoader::load_map_area(0, m_map_area);

	// create terrain vertex buffer

	m_vertex_buffer = 
		new Renderer2D::TriVertex[((uint64_t)m_map_info.size - 1) * ((uint64_t)m_map_info.size - 1) * 2 * 3];
	Renderer2D::TriVertex* vertex_buffer_ptr = (Renderer2D::TriVertex*)m_vertex_buffer;

	for (uint32_t y = 0; y < m_map_info.size - 1; y++)
	{
		for (uint32_t x = 0; x < m_map_info.size - 1; x++)
		{
			//      (0) (3)--- 4
			//      / \   \ B /
			//     / A \   \ /
			//    2 --- 1   5
			//   /     /   /
			//  /     /   /
			// 6     7   7

			MapNode& n0 = m_map_area[(m_map_info.size * y) + x];
			MapNode& n1 = m_map_area[(m_map_info.size * (y + 1)) + x + 1];
			MapNode& n2 = m_map_area[(m_map_info.size * (y + 1)) + x];
			MapNode& n3 = n0;
			MapNode& n4 = m_map_area[(m_map_info.size * y) + x + 1];
			MapNode& n5 = n1;	

			// TODO: maybe not work on HEAP here
			// doesn't matter for now as this is executed once

			Renderer2D::TriVertex& v0 = vertex_buffer_ptr[0];
			Renderer2D::TriVertex& v1 = vertex_buffer_ptr[1];
			Renderer2D::TriVertex& v2 = vertex_buffer_ptr[2];
			Renderer2D::TriVertex& v3 = vertex_buffer_ptr[3];
			Renderer2D::TriVertex& v4 = vertex_buffer_ptr[4];
			Renderer2D::TriVertex& v5 = vertex_buffer_ptr[5];

			v0.pos = gridpos_to_world(x, y, m_map_info.size);
			v1.pos = gridpos_to_world(x + 1, y + 1, m_map_info.size);
			v2.pos = gridpos_to_world(x, y + 1, m_map_info.size);
			v3.pos = v0.pos;
			v4.pos = gridpos_to_world(x + 1, y, m_map_info.size);
			v5.pos = v1.pos;

			// TODO: map texture coords from atlas to vertices based on GroundType
			/*uint32_t tex_id_a = tex_id_from_types(n0.type, n3.type, n2.type);
			uint32_t tex_id_b = tex_id_from_types(n0.type, n1.type, n3.type);*/

			v0.tex = { v0.pos.x / 128.0f, v0.pos.y / 128.0f };
			v1.tex = { v1.pos.x / 128.0f, v1.pos.y / 128.0f };
			v2.tex = { v2.pos.x / 128.0f, v2.pos.y / 128.0f };
			v3.tex = { v3.pos.x / 128.0f, v3.pos.y / 128.0f };
			v4.tex = { v4.pos.x / 128.0f, v4.pos.y / 128.0f };
			v5.tex = { v5.pos.x / 128.0f, v5.pos.y / 128.0f };

			v0.tex_index = 1.0f;
			v1.tex_index = 1.0f;
			v2.tex_index = 1.0f;
			v3.tex_index = 1.0f;
			v4.tex_index = 1.0f;
			v5.tex_index = 1.0f;

			// row shifting x position

			v0.pos.x += (m_map_info.size - y + 1) * 8;
			v3.pos.x += (m_map_info.size - y + 1) * 8;
			v4.pos.x += (m_map_info.size - y + 1) * 8;
			v1.pos.x += (m_map_info.size - y) * 8;
			v2.pos.x += (m_map_info.size - y) * 8;
			v5.pos.x += (m_map_info.size - y) * 8;

			// height shifting y position

			v0.pos.y += n0.height;
			v1.pos.y += n1.height;
			v2.pos.y += n2.height;
			v3.pos.y += n3.height;
			v4.pos.y += n4.height;
			v5.pos.y += n5.height;

			// shading

			v0.shade = 0.875f + (float)(n0.height - n2.height) * 0.1f;
			v3.shade = v0.shade;
			v4.shade = 0.875f + (float)(n4.height - n5.height) * 0.1f;

			if (y < m_map_info.size - 2)
			{
				MapNode& n6 = m_map_area[(m_map_info.size * (y + 2)) + x];
				MapNode& n7 = m_map_area[(m_map_info.size * (y + 2)) + x + 1];

				v1.shade = 0.875f + (float)(n1.height - n7.height) * 0.1f;
				v2.shade = 0.875f + (float)(n2.height - n6.height) * 0.1f;
				v5.shade = v1.shade;
			}
			else 
			{
				// n6 & n7 are out of range
				v1.shade = 1.0f;
				v2.shade = 1.0f;
				v5.shade = 1.0f;
			}

			vertex_buffer_ptr += 6;
		}
	}
}

void TerrainLayer::on_detach()
{
	delete[] m_map_area;
	delete[] m_vertex_buffer;
}

void TerrainLayer::on_update(TimeDelta time_delta)
{
	m_camera_controller.on_update(time_delta);

	Renderer2D::begin_scene(m_camera_controller.get_camera());
	Renderer2D::submit(
		m_vertex_buffer,
		((uint64_t)m_map_info.size - 1) * ((uint64_t)m_map_info.size - 1) * 2 * 3,
		m_tex_terrain[0] );
	Renderer2D::end_scene();
}

void TerrainLayer::on_event(Event& e)
{
	m_camera_controller.on_event(e);
}