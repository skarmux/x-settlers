#include "layer/terrain_layer.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <math.h>

inline static glm::vec2 gridpos_to_world(uint32_t x, uint32_t y, uint32_t width)
{
	constexpr uint32_t GRID_X_STEP = 16;
	constexpr uint32_t GRID_Y_STEP = 8;

	return glm::vec2(x * GRID_X_STEP, (width - y) * GRID_Y_STEP);
}

glm::vec2* TerrainLayer::tex_coords_from_types(
	uint8_t t0, uint8_t t1, uint8_t t2,
	const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2 ) const
{
	glm::vec2 tex_coords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {0.0f,1.0f} };

	int32_t tex_id = -1;

	if (t0 == t1 && t1 == t2)
	{
		// 128 x 128 FULL

		tex_coords[0] = {
			std::fmod(p0.x, 128.0f) / 128.0f,
			1.0f - std::fmod(p0.y, 128.0f) / 128.0f };
		tex_coords[1] = {
			std::fmod(p1.x, 128.0f) / 128.0f,
			1.0f - std::fmod(p1.y, 128.0f) / 128.0f };
		tex_coords[2] = {
			std::fmod(p2.x, 128.0f) / 128.0f,
			1.0f - std::fmod(p2.y, 128.0f) / 128.0f };

		// handle overflow condition

		float& u0 = tex_coords[0].x;
		float& u1 = tex_coords[1].x;
		float& u2 = tex_coords[2].x;
		float& v0 = tex_coords[0].y;
		float& v1 = tex_coords[1].y;
		float& v2 = tex_coords[2].y;

		if (u0 > 1.0f || u1 > 1.0f || u2 > 1.0f || v0 > 1.0f || v1 > 1.0f || v2 > 1.0f)
			RENDERER_ERROR("texture coordinate out of range");

		if (u0 < 0.0f || u1 < 0.0f || u2 < 0.0f || v0 < 0.0f || v1 < 0.0f || v2 < 0.0f)
			RENDERER_ERROR("texture coordinate out of range");

		// 0     0 --- 1   v|y ^
		// | \     \ B |       |
		// | A \     \ |       |
		// 2 --- 1     2     0 +---- > u|x

		// handle edge cases
		if (p0.y != p1.y)
		{
			if (u2 == 1.0f && u2 > u1) // A0
			{
				u0 = 0.0f;
				u2 = 0.0f;
			}
			else if (u1 == 0.0f && u1 < u2) // A1
			{
				u1 = 1.0f;
			}

			if (v0 == 1.0f && v0 > v2) // A2
			{
				v0 = 0.0f;
			}
			else if (v2 == 0.0f && v2 < v0) // A3
			{
				v2 = 1.0f;
				v1 = 1.0f;
			}
		}
		else
		{
			if (u1 == 0.0f && u1 < u0) // B0
			{
				u1 = 1.0f;
				u2 = 1.0f;
			}
			else if (u0 == 1.0f && u0 > u1) // B1
			{
				u0 = 0.0f;
			}

			if (v1 == 1.0f && v1 > v2) // B2
			{
				v0 = 0.0f;
				v1 = 0.0f;
			}
			else if (v2 == 0.0f && v2 < v1) // B3
			{
				v2 = 1.0f;
			}
		}

		switch (t0)
		{
		case 7:   tex_id = 10; break; // SEA
		case 16: // GRASS
		{
			tex_id = 0;
			break; 
		}
		case 32:  tex_id = 21; break; // ROCK
		case 48:  tex_id = 31; break; // BEACH
		case 64:  tex_id = 18; break; // DESERT
		case 80:  tex_id =  7; break; // SWAMP
		case 128: tex_id = 24; break; // SNOW/ICE
		case 144: tex_id =  4; break; // MUD
		}

		if (tex_id >= 0)
		{
			m_atlas.translate_to_atlas_coords(tex_id, tex_coords[0]);
			m_atlas.translate_to_atlas_coords(tex_id, tex_coords[1]);
			m_atlas.translate_to_atlas_coords(tex_id, tex_coords[2]);
		}		
	}

	return tex_coords;
}

void TerrainLayer::on_attach()
{
	MapLoader::init();

	{
		// fetch MapData for each map file within 'assets/maps/' folder
		const std::string texture_folder = "assets/textures/s3/terrain/";
		for (uint32_t i = 0; i < 235; i++)
		{
			uint32_t tmp_id;
			m_atlas.add_texture(texture_folder + std::to_string(i) + ".png");
		}

		m_atlas_texture = m_atlas.create_texture(1024, 1024);
	}

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

	m_map_info = maps[0]; // select first map from selection

	m_map_area = new MapNode[(uint64_t)m_map_info.size * (uint64_t)m_map_info.size];
	MapLoader::load_map_area(0, m_map_area);

	// fill terrain vertex buffer

	m_vertex_buffer = 
		new Renderer2D::TriVertex[((uint64_t)m_map_info.size - 1) * ((uint64_t)m_map_info.size - 1) * 2 * 3];
	Renderer2D::TriVertex* vertex_buffer_ptr = (Renderer2D::TriVertex*)m_vertex_buffer;

	for (uint32_t y = 0; y < m_map_info.size - 1; y++)
	{
		for (uint32_t x = 0; x < m_map_info.size - 1; x++)
		{
			//      (0) (3) -- 4
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

			// looking up texture coordinates

			glm::vec2* tex_coords_a = 
				tex_coords_from_types(
					n0.type, n1.type, n2.type,
					v0.pos, v1.pos, v2.pos);
			v0.tex = tex_coords_a[0];
			v1.tex = tex_coords_a[1];
			v2.tex = tex_coords_a[2];

			glm::vec2* tex_coords_b = 
				tex_coords_from_types(
					n3.type, n4.type, n5.type,
					v3.pos, v4.pos, v5.pos);
			v3.tex = tex_coords_b[0];
			v4.tex = tex_coords_b[1];
			v5.tex = tex_coords_b[2];

			// TODO: stop using texture indices, and use one single tex atlas instead
			v0.tex_index = 1.0f;
			v1.tex_index = 1.0f;
			v2.tex_index = 1.0f;
			v3.tex_index = 1.0f;
			v4.tex_index = 1.0f;
			v5.tex_index = 1.0f;

			// row-shifting x position

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
		m_atlas_texture);
	Renderer2D::end_scene();
}

void TerrainLayer::on_event(Event& e)
{
	m_camera_controller.on_event(e);
}