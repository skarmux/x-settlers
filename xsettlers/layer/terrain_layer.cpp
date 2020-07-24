#include "layer/terrain_layer.h"

#include <glm/vec2.hpp> /* vec2 */

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

	for (uint32_t row = 0; row < m_map_info.size - 1; row++)
	{
		for (uint32_t col = 0; col < m_map_info.size - 1; col++)
		{
			// 0     3 --- 4     +----> u|x
			// | \     \ B |     |
			// | A \     \ |     |
			// 2 --- 1     5     v v|y
			// |     |     |
			// 6     7     7

			MapNode& n0 = m_map_area[(m_map_info.size * row) + col];
			MapNode& n1 = m_map_area[(m_map_info.size * (row + 1)) + col + 1];
			MapNode& n2 = m_map_area[(m_map_info.size * (row + 1)) + col];
			MapNode& n3 = n0;
			MapNode& n4 = m_map_area[(m_map_info.size * row) + col + 1];
			MapNode& n5 = n1;

			// TODO: maybe not work on HEAP here
			// doesn't matter for now as this is executed once

			Renderer2D::TriVertex& v0 = vertex_buffer_ptr[0];
			Renderer2D::TriVertex& v1 = vertex_buffer_ptr[1];
			Renderer2D::TriVertex& v2 = vertex_buffer_ptr[2];
			Renderer2D::TriVertex& v3 = vertex_buffer_ptr[3];
			Renderer2D::TriVertex& v4 = vertex_buffer_ptr[4];
			Renderer2D::TriVertex& v5 = vertex_buffer_ptr[5];

			// setting up texture(-atlas) coordinates

			// 0     3 --- 4     +----> u|x
			// | \     \ B |     |
			// | A \     \ |     |
			// 2 --- 1     5     v v|y

			glm::vec2* tex_coords_a = 
				tex_coords_from_types(
					n0.type,
					n1.type,
					n2.type,
					{ col, row },         // 0
					{ col + 1, row + 1 }, // 1
					{ col, row + 1 }      // 2
			);

			v0.tex = tex_coords_a[0];
			v1.tex = tex_coords_a[1];
			v2.tex = tex_coords_a[2];

			glm::vec2* tex_coords_b =
				tex_coords_from_types(
					n3.type,
					n4.type,
					n5.type,
					{ col, row },        // 3
					{ col + 1, row },    // 4
					{ col + 1, row + 1 } // 5
			);

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

			// transforming grid location to world position

			v0.pos = gridpos_to_worldpos(col, row, m_map_info.size);
			v1.pos = gridpos_to_worldpos(col + 1, row + 1, m_map_info.size);
			v2.pos = gridpos_to_worldpos(col, row + 1, m_map_info.size);
			v3.pos = v0.pos;
			v4.pos = gridpos_to_worldpos(col + 1, row, m_map_info.size);
			v5.pos = v1.pos;

			// row-shifting x position

			v0.pos.x += (m_map_info.size - row + 1) * 8;
			v3.pos.x += (m_map_info.size - row + 1) * 8;
			v4.pos.x += (m_map_info.size - row + 1) * 8;
			v1.pos.x += (m_map_info.size - row) * 8;
			v2.pos.x += (m_map_info.size - row) * 8;
			v5.pos.x += (m_map_info.size - row) * 8;

			// --> (0) (3) -- 4
			//     / \   \ B /
			//    / A \   \ /
			//   2 --- 1   5

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

			if (row < m_map_info.size - 2)
			{
				MapNode& n6 = m_map_area[(m_map_info.size * (row + 2)) + col];
				MapNode& n7 = m_map_area[(m_map_info.size * (row + 2)) + col + 1];

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

inline glm::vec2 TerrainLayer::gridpos_to_worldpos(uint32_t x, uint32_t y, uint32_t width) const
{
	constexpr uint32_t GRID_X_STEP = 16;
	constexpr uint32_t GRID_Y_STEP = 8;

	return glm::vec2(x * GRID_X_STEP, (width - y) * GRID_Y_STEP);
}

glm::vec2* TerrainLayer::tex_coords_from_types(
	uint8_t type_0,
	uint8_t type_1,
	uint8_t type_2,
	const glm::ivec2& gridpos_0,
	const glm::ivec2& gridpos_1,
	const glm::ivec2& gridpos_2) const
{
	glm::vec2 tex_coords[3];

	int32_t tex_id = -1;

	if (type_0 == type_1 && type_1 == type_2)
	{
		float tex_segments; // (8)128x128 (2)32x32

		switch (type_0)
		{
		case   0: tex_id = 17; tex_segments = 2.0f; break; // Sea 0
		case   1: tex_id = 16; tex_segments = 2.0f; break; // Sea 1
		case   2: tex_id = 15; tex_segments = 2.0f; break; // Sea 2
		case   3: tex_id = 14; tex_segments = 2.0f; break; // Sea 3
		case   4: tex_id = 13; tex_segments = 2.0f; break; // Sea 4
		case   5: tex_id = 12; tex_segments = 2.0f; break; // Sea 5
		case   6: tex_id = 11; tex_segments = 2.0f; break; // Sea 6
		case   7: tex_id = 10; tex_segments = 8.0f; break; // SEA
		case  16: tex_id =  0; tex_segments = 8.0f; break; // GRASS
		case  17: tex_id = 23; tex_segments = 2.0f; break; // Rock
		case  20: tex_id = 20; tex_segments = 2.0f; break; // Desert
		case  21: tex_id =  8; tex_segments = 2.0f; break; // Swamp
		case  32: tex_id = 21; tex_segments = 8.0f; break; // ROCK
		case  33: tex_id = 22; tex_segments = 2.0f; break; // Rock
		case  35: tex_id = 25; tex_segments = 2.0f; break; // Ice
		case  48: tex_id = 31; tex_segments = 8.0f; break; // BEACH
		case  64: tex_id = 18; tex_segments = 8.0f; break; // DESERT
		case  65: tex_id = 19; tex_segments = 2.0f; break; // Desert
		case  80: tex_id =  7; tex_segments = 8.0f; break; // SWAMP
		case  81: tex_id =  9; tex_segments = 2.0f; break; // Swamp
		case 128: tex_id = 24; tex_segments = 8.0f; break; // SNOW/ICE
		case 129: tex_id = 26; tex_segments = 2.0f; break; // Ice
		case 144: tex_id =  4; tex_segments = 8.0f; break; // MUD
		default:
			CORE_WARN("ground type not mapped: {0}", type_0);
		}
		
		// 128x128 REPEAT

		tex_coords[0] = {
			(float)(gridpos_0.x % (int)tex_segments) / tex_segments,
			(float)(gridpos_0.y % (int)tex_segments) / tex_segments };

		// 0     0 --- 1     +----> u|x
		// | \     \ B |     |
		// | A \     \ |     |
		// 2 --- 1     2     v v|y

		if (gridpos_0.y != gridpos_1.y)
		{
			// Triangle A
			tex_coords[1] = {
				tex_coords[0].x + (1.0f / tex_segments),
				tex_coords[0].y + (1.0f / tex_segments) };
			tex_coords[2] = {
				tex_coords[0].x,
				tex_coords[0].y + (1.0f / tex_segments) };
		}
		else
		{
			// Triangle B
			tex_coords[1] = {
				tex_coords[0].x + (1.0f / tex_segments),
				tex_coords[0].y };
			tex_coords[2] = {
				tex_coords[0].x + (1.0f / tex_segments),
				tex_coords[0].y + (1.0f / tex_segments) };
		}
	}
	else if (type_0 == type_1 || type_1 == type_2 || type_2 == type_0) // Hexagonal
	{
		//     0     0 --- 1    0 +---- > u/x
		//    / \     \ B /       |
		//   / A \     \ /        |
		//  2 --- 1     2     v/y v

		uint8_t type_center;
		uint8_t type_border;

		if (type_0 == type_1)
		{
			type_center = type_2; type_border = type_0;
			
			tex_coords[0] = { 
				0.5f + ((gridpos_0.x - gridpos_2.x) * 0.5f + (gridpos_0.y - gridpos_2.y) * -0.25f),
				0.5f + ((gridpos_0.y - gridpos_2.y) * 0.5f) };

			tex_coords[1] = {
				0.5f + ((gridpos_1.x - gridpos_2.x) * 0.5f + (gridpos_1.y - gridpos_2.y) * -0.25f),
				0.5f + ((gridpos_1.y - gridpos_2.y) * 0.5f) };

			tex_coords[2] = { 0.5f, 0.5f };
		}			
		else if (type_1 == type_2)
		{
			type_center = type_0; type_border = type_1;
			
			tex_coords[0] = { 0.5f, 0.5f };

			tex_coords[1] = {
				0.5f + ((gridpos_1.x - gridpos_0.x) * 0.5f + (gridpos_1.y - gridpos_0.y) * -0.25f),
				0.5f + ((gridpos_1.y - gridpos_0.y) * 0.5f) }; // = { .75, 1 }

			tex_coords[2] = {
				0.5f + ((gridpos_2.x - gridpos_0.x) * 0.5f + (gridpos_2.y - gridpos_0.y) * -0.25f),
				0.5f + ((gridpos_2.y - gridpos_0.y) * 0.5f) }; // = { .25, 1 }
		}
		else
		{
			type_center = type_1; type_border = type_0;

			tex_coords[0] = {
				0.5f + ((gridpos_0.x - gridpos_1.x) * 0.5f + (gridpos_0.y - gridpos_1.y) * -0.25f),
				0.5f + ((gridpos_0.y - gridpos_1.y) * 0.5f) };

			tex_coords[1] = { 0.5f, 0.5f };

			tex_coords[2] = {
				0.5f + ((gridpos_2.x - gridpos_1.x) * 0.5f + (gridpos_2.y - gridpos_1.y) * -0.25f),
				0.5f + ((gridpos_2.y - gridpos_1.y) * 0.5f) };
		}

		switch (type_center) {
		case 0: // Sea 0
			switch (type_border) {
			case  1: tex_id = 84; break;
			case 48: tex_id = 37; break; // Beach
			}
			break;
		case 1: // Sea 1
			switch (type_border) {
			case 0: tex_id = 86; break;
			case 2: tex_id = 88; break;
			}
		case 2: // Sea 2
			switch (type_border) {
			case 1: tex_id = 90; break;
			case 3: tex_id = 92; break;
			}
		case 3: // Sea 3
			switch (type_border) {
			case 2: tex_id = 94; break;
			case 4: tex_id = 96; break;
			}
		case 4: // Sea 4
			switch (type_border) {
			case 3: tex_id = 98; break;
			case 5: tex_id = 100; break;
			}
		case 5: // Sea 5
			switch (type_border) {
			case 4: tex_id = 102; break;
			case 6: tex_id = 104; break;
			}
		case 6: // Sea 6
			switch (type_border) {
			case 5: tex_id = 106; break;
			case 7: tex_id = 108; break;
			}
		case 7: // Sea 7
			switch (type_border) {
			case 6: tex_id = 110; break;
			}
		case 16: // Grass
			switch (type_border) {
			case 17: tex_id = 116; break;
			case 48: tex_id = 112; break; // Beach
			case 96: tex_id = 54; break;
			case 97: tex_id = 58; break;
			case 98: tex_id = 62; break;
			case 99: tex_id = 66; break;
			}
			break;
		case 17: // Rock
			switch (type_border) {
			case 16: tex_id = 118; break;
			case 33: tex_id = 120; break;
			}
		case 32:
			switch (type_border) {
			case 33: tex_id = 126; break;
			}
		case 33:
			switch (type_border) {
			case 17: tex_id = 122; break;
			case 32: tex_id = 124; break;
			}
		case 48: // Beach
			switch (type_border) {
			case  0: tex_id =  39; break; // Sea
			case 16: tex_id = 114; break; // Grass
			}
			break;
		case 96: // River
			switch (type_border) {
			case 16: tex_id = 52; break; // Grass
			}
			break;
		case 97: // River
			switch (type_border) {
			case 16: tex_id = 56; break; // Grass
			}
			break;
		case 98: // River
			switch (type_border) {
			case 16: tex_id = 60; break; // Grass
			}
			break;
		case 99: // River
			switch (type_border) {
			case 16: tex_id = 64; break; // Grass
			}
			break;
		}

		tex_id += (gridpos_0.x + gridpos_0.y) % 2; // (gridpos_0.x + gridpos_0.y) % 2
	}
	else
	{
		RENDERER_WARN("three individual types in one triangle {0}, {1}, {2}", type_0, type_1, type_2);
	}

	if (tex_id > -1)
	{
		m_atlas.translate_to_atlas_coords(tex_id, tex_coords[0]);
		m_atlas.translate_to_atlas_coords(tex_id, tex_coords[1]);
		m_atlas.translate_to_atlas_coords(tex_id, tex_coords[2]);
	}

	return tex_coords;
}