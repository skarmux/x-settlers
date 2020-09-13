#include "layer/terrain_layer.h"

#include <glm/vec2.hpp>

enum class S3Field
{
	Sea0 = 0,
	Sea1 = 1,
	Sea2 = 2,
	Sea3 = 3,
	Sea4 = 4,
	Sea5 = 5,
	Sea6 = 6,
	Sea7 = 7,

	Grass = 16,
	Field = 17,
	GrassRock = 18, // unused ?
	GrassRiver = 19,
	GrassDesert = 20,
	GrassSwamp = 21,
	// = 22 // ?
	GrassMud = 23,

	Rock = 32,
	RockGrass = 33,
	RockSnow = 35,

	Beach = 48,
	BeachRiver = 49, // ?

	Desert = 64,
	DesertGrass = 65,
	DesertBeach0 = 66, // ?
	DesertBeach1 = 67, // ?

	Swamp = 80,
	SwampGrass = 81,
	SwampRock = 83,

	River0 = 96,
	River1 = 97,
	River2 = 98,
	River3 = 99,

	Snow = 128,
	SnowRock = 129,

	Mud = 144,
	MudGrass = 145
};

static uint32_t s3_tex_id(S3Field center, S3Field edge)
{
	int32_t tex_id = -1;

	switch (center) {
	case S3Field::Sea0:
		     if (edge == S3Field::Sea0)   tex_id =  17;
		else if (edge == S3Field::Sea1)   tex_id =  86;
		else if (edge == S3Field::Beach)  tex_id =  37;
		else if (edge == S3Field::River3) tex_id = 176; // TODO
		break;
	case S3Field::Sea1:
		     if (edge == S3Field::Sea0) tex_id =  84;
		else if (edge == S3Field::Sea1) tex_id =  16;
		else if (edge == S3Field::Sea2) tex_id =  90;
		break;
	case S3Field::Sea2:					    
		     if (edge == S3Field::Sea1) tex_id =  88;
		else if (edge == S3Field::Sea2) tex_id =  15;
		else if (edge == S3Field::Sea3) tex_id =  94;
		break;
	case S3Field::Sea3:					    
		     if (edge == S3Field::Sea2) tex_id =  92;
		else if (edge == S3Field::Sea3) tex_id =  14;
		else if (edge == S3Field::Sea4) tex_id =  98;
		break;
	case S3Field::Sea4:
		     if (edge == S3Field::Sea3) tex_id =  96;
		else if (edge == S3Field::Sea4) tex_id =  13;
		else if (edge == S3Field::Sea5) tex_id = 102;
		break;
	case S3Field::Sea5:
		     if (edge == S3Field::Sea4) tex_id = 100;
		else if (edge == S3Field::Sea5) tex_id =  12;
		else if (edge == S3Field::Sea6) tex_id = 106;
		break;
	case S3Field::Sea6:
		     if (edge == S3Field::Sea5) tex_id = 104;
		else if (edge == S3Field::Sea6) tex_id =  11;
		else if (edge == S3Field::Sea7) tex_id = 110;
		break;
	case S3Field::Sea7:
		     if (edge == S3Field::Sea6) tex_id = 108;
		else if (edge == S3Field::Sea7) tex_id =  10;
		break;
	case S3Field::Grass:
		     if (edge == S3Field::Grass)       tex_id =   0;
		else if (edge == S3Field::Field)       tex_id = 118;
		else if (edge == S3Field::GrassDesert) tex_id = 130;
		else if (edge == S3Field::GrassSwamp)  tex_id = 203;
		else if (edge == S3Field::GrassMud)    tex_id = 142;
		else if (edge == S3Field::Beach)       tex_id = 112;
		else if (edge == S3Field::River0)      tex_id =  54;
		else if (edge == S3Field::River1)      tex_id =  58;
		else if (edge == S3Field::River2)      tex_id =  62;
		else if (edge == S3Field::River3)      tex_id =  66;
		break;
	case S3Field::Field:
		     if (edge == S3Field::Grass)     tex_id = 116;
		else if (edge == S3Field::Field)     tex_id =   1;
		else if (edge == S3Field::RockGrass) tex_id = 122;
		break;
	case S3Field::GrassRiver:
		if (edge == S3Field::River0) tex_id = 176; // TODO
		break;
	case S3Field::GrassDesert:
		     if (edge == S3Field::Grass)       tex_id = 128;
		else if (edge == S3Field::GrassDesert) tex_id =  20;
		else if (edge == S3Field::DesertGrass) tex_id = 134;
		break;
	case S3Field::GrassSwamp:
		     if (edge == S3Field::GrassSwamp) tex_id =   8;
		else if (edge == S3Field::Grass)      tex_id = 201;
		else if (edge == S3Field::SwampGrass) tex_id = 207;
		break;
	case S3Field::GrassMud:
		     if (edge == S3Field::Grass)    tex_id = 140;
		else if (edge == S3Field::MudGrass) tex_id = 146;
		break;
	case S3Field::GrassRock:
		if (edge == S3Field::GrassRock)     tex_id = 23;
		break;
	case S3Field::Rock:
		     if (edge == S3Field::Rock)      tex_id =  21;
		else if (edge == S3Field::RockGrass) tex_id = 126;
		break;
	case S3Field::RockGrass:
		     if (edge == S3Field::RockGrass) tex_id =  22;
		else if (edge == S3Field::Field)     tex_id = 120;
		else if (edge == S3Field::Rock)      tex_id = 124;
		break;
	case S3Field::RockSnow:
		if (edge == S3Field::RockSnow) tex_id = 25;
		break;
	case S3Field::Beach:
		     if (edge == S3Field::Beach)  tex_id =  31;
		else if (edge == S3Field::Sea0)   tex_id =  39;
		else if (edge == S3Field::Grass)  tex_id = 114;
		else if (edge == S3Field::River0) tex_id =  70;
		else if (edge == S3Field::River1) tex_id =  74;
		else if (edge == S3Field::River2) tex_id =  78;
		else if (edge == S3Field::River3) tex_id =  82;
		break;
	case S3Field::BeachRiver:
		if (edge == S3Field::River2) tex_id = 176; // TODO
		break;
	case S3Field::River0:
		     if (edge == S3Field::Grass) tex_id = 52;
		else if (edge == S3Field::Beach) tex_id = 68;
		break;
	case S3Field::River1:
		     if (edge == S3Field::Grass) tex_id = 56;
		else if (edge == S3Field::Beach) tex_id = 72;
		break;
	case S3Field::River2:
		     if (edge == S3Field::Grass) tex_id = 60;
		else if (edge == S3Field::Beach) tex_id = 76;
		break;
	case S3Field::River3:
		     if (edge == S3Field::Grass) tex_id =  64;
		else if (edge == S3Field::Beach) tex_id =  80;
		else if (edge == S3Field::Sea0)  tex_id = 176; // TODO
		break;
	case S3Field::Desert:
		     if (edge == S3Field::Desert)      tex_id =  18;
		else if (edge == S3Field::DesertGrass) tex_id = 136; //TODO
		break;
	case S3Field::DesertGrass:
		     if (edge == S3Field::Desert)      tex_id = 138;
		else if (edge == S3Field::DesertGrass) tex_id =  19;
		else if (edge == S3Field::GrassDesert) tex_id = 132;
		break;
	case S3Field::DesertBeach0:
		if (edge == S3Field::Beach) tex_id = 176; // TODO
		break;
	case S3Field::DesertBeach1:
		if (edge == S3Field::Beach) tex_id = 176; // TODO
		break;
	case S3Field::Swamp:
		     if (edge == S3Field::Swamp)      tex_id =   7;
		else if (edge == S3Field::SwampGrass) tex_id = 209;
		break;
	case S3Field::SwampGrass:
		     if (edge == S3Field::Swamp)      tex_id = 211;
		else if (edge == S3Field::GrassSwamp) tex_id = 205;
		else if (edge == S3Field::SwampGrass) tex_id =   9;
		break;
	case S3Field::SwampRock:
		if (edge == S3Field::Rock) tex_id = 176; // TODO
		break;
	case S3Field::Snow:
		if (edge == S3Field::Snow) tex_id = 24;
		break;
	case S3Field::SnowRock:
		if (edge == S3Field::SnowRock) tex_id = 26;
		break;
	case S3Field::Mud:
		     if (edge == S3Field::Mud)      tex_id =   4;
		else if (edge == S3Field::MudGrass) tex_id = 148;
		break;
	case S3Field::MudGrass:
		     if (edge == S3Field::Mud)      tex_id = 150;
		else if (edge == S3Field::GrassMud) tex_id = 144;
		break;
	}

	if (tex_id < 0) {
		CORE_ERROR("No tex mapping {0}(center) {1}(edge)", center, edge);
		return 0;
	}

	return (uint32_t)tex_id;
}

void TerrainLayer::on_attach()
{
	TextureAtlas atlas = TextureAtlas(Texture::Format::RGB);

	const std::string texture_folder = "assets/textures/s3/terrain/";
	for (uint32_t i = 0; i < 235; i++)
	{
		std::string path = texture_folder + std::to_string(i) + ".png";
		switch (i) {
		case  0: case  1: case   2: case   3: case  4: case 7:
		case 10: case 18: case  21: case  24: case 31:
		case 35: case 36: case 217: case 230:
		{
			// add 8 texels to texture width
			uint32_t src_width, height, channels, atlas_channels = 3;
			uint8_t* src_data_ptr = Filesystem::image_load(path.c_str(), &src_width, &height, &channels, atlas_channels);
			uint8_t* dst_data_ptr = new uint8_t[((uint64_t)src_width + 8) * (uint64_t)height * (uint64_t)atlas_channels];

			size_t dst_offset = 0, src_offset = 0;

			for (uint32_t row = 0; row < height; row++)
			{
				memcpy(dst_data_ptr + dst_offset, src_data_ptr + src_offset,
					(uint64_t)src_width * (uint64_t)3 * sizeof(uint8_t));

				dst_offset += (uint64_t)src_width * (uint64_t)atlas_channels;

				memcpy(dst_data_ptr + dst_offset, src_data_ptr + src_offset,
					(uint64_t)8 * (uint64_t)atlas_channels * sizeof(uint8_t));

				dst_offset += 8 * (uint64_t)atlas_channels;
				src_offset += (uint64_t)src_width * (uint64_t)atlas_channels;
			}

			Filesystem::image_free(src_data_ptr);

			atlas.add_image(dst_data_ptr, src_width + 8, height, Texture::Format::RGB);
		}
			break;
		default:
			atlas.add_image(path);
		}
	}

	m_atlas = atlas.create_texture(1024, 1024);

	// =============================================================================

	constexpr glm::uvec2 seg_size   = glm::uvec2(8,16);
	constexpr glm::uvec2 seg_repeat = glm::uvec2(16,8);
	constexpr glm::uvec2 seg_patch  = glm::uvec2(4,2);
	constexpr glm::uvec2 seg_tile   = glm::uvec2(2,1);

	m_tilemap = Texture2D::create((m_map_size - 1), (m_map_size - 1), Texture::Format::RGBA, Texture::Type::UNSIGNED_INT);
	uint8_t* const tilemap_data = new uint8_t[4 * (uint64_t)(m_map_size - 1) * (uint64_t)(m_map_size - 1)];
	uint8_t* tilemap_data_ptr = tilemap_data;

	for (uint32_t row = 0; row < (m_map_size - 1); row++)
	{
		for (uint32_t col = 0; col < (m_map_size - 1); col++)
		{
			MapNode& n0 = m_nodes[(m_map_size * row) + col];
			MapNode& n1 = m_nodes[(m_map_size * row) + col + 1];
			MapNode& n2 = m_nodes[(m_map_size * (row + 1)) + col];
			MapNode& n3 = m_nodes[(m_map_size * (row + 1)) + col + 1];

			// TODO: add extra case for when there are three unique types

			uint8_t center = n0.type ^ n3.type ^ n2.type;
			uint8_t edge = center ^ ((n0.type ^ center) | (n3.type ^ center) | (n2.type ^ center));
			uint32_t texture_id;

			if (n0.type != n3.type && n0.type != n2.type && n3.type != n2.type)
				texture_id = 176;
			else
				texture_id = s3_tex_id(static_cast<S3Field>(center), static_cast<S3Field>(edge));

			for (uint32_t i = 0; i < 2; i++)
			{
				// R-triangle (i == 1)
				
				glm::uvec2 tile_pos = glm::uvec2(0,0);

				if (center == edge)
				{
					tile_pos.x += i;

					switch (static_cast<S3Field>(center)) {
					//  + o---o---o
					//   / \ / \ /
					//  o---o---o +  REPEAT
					//   \ / \ / \
					//  + o---o---o
					
					case S3Field::Grass: case S3Field::Sea7:
						// apply brick pattern
						tile_pos.x += (((row / seg_repeat.y) % 2) * seg_repeat.y);

					case S3Field::Rock: case S3Field::Beach: case S3Field::Desert:
					case S3Field::Swamp: case S3Field::Snow: case S3Field::Mud:
						tile_pos.x += (col * seg_tile.x) + (row * (seg_repeat.x - 1));
						tile_pos.x %= seg_repeat.x;
						tile_pos.y += row % seg_repeat.y;
						break;

					default:
						//  + o   o +
						//   / \ / \
						//  o---o---o  PATCH
						//   \ / \ /
						//  + o   o +
						tile_pos.x += (col * seg_tile.x);
						tile_pos.x %= seg_patch.x;
						tile_pos = glm::uvec2(0,0); // TODO
					}
				}
				else
				{
					//  + e---e +
					//   / \ / \
					//  e---c---e  TRANSITION
					//   \ / \ /
					//  + e---e +
					if (n0.type == center)
						tile_pos = glm::uvec2(1 + i, 1);
					else if (n3.type == center)
						tile_pos = glm::uvec2(i, 0);
					else if (i == 1) // n1.type == center
						tile_pos = glm::uvec2(0, 1);
					else if (i == 0) // n2.type == center
						tile_pos = glm::uvec2(2, 0);

					// vary texture for every next grid cell (checkerboard)
					texture_id += (col + row) % 2;
				}

				tile_pos += atlas.texel_offset(texture_id) / seg_size;

				*tilemap_data_ptr++ = (uint8_t)tile_pos.x;
				*tilemap_data_ptr++ = (uint8_t)tile_pos.y;

				center = n0.type ^ n1.type ^ n3.type;
				edge = center ^ ((n0.type ^ center) | (n1.type ^ center) | (n3.type ^ center));

				if (n0.type != n3.type && n0.type != n1.type && n3.type != n1.type)
					texture_id = 176;
				else
					texture_id = s3_tex_id(static_cast<S3Field>(center), static_cast<S3Field>(edge));
			}
		}
	}

	m_tilemap->push_data(tilemap_data, (m_map_size - 1), (m_map_size - 1),
		Texture::Format::RGBA, Texture::Type::UNSIGNED_INT);

	delete[] tilemap_data;

	// =============================================================================

	m_heightmap = Texture2D::create(m_map_size, m_map_size, Texture::Format::RED, Texture::Type::UNSIGNED_BYTE);
	m_heightmap->set_filter(Texture::Filter::LINEAR);
	uint8_t* const heightmap = new uint8_t[(uint64_t)m_map_size * (uint64_t)m_map_size];
	uint8_t* heightmap_ptr = heightmap;

	for (uint32_t row = 0; row < m_map_size; row++)
	{
		for (uint32_t col = 0; col < m_map_size; col++)
		{
			MapNode& node = m_nodes[(m_map_size * row) + col];
			*heightmap_ptr++ = node.height;// *(0xFF / 85); // normalize to [0,1] range with bitdepth 8
		}
	}

	m_heightmap->push_data(heightmap, m_map_size, m_map_size, Texture::Format::RED, Texture::Type::UNSIGNED_BYTE);

	delete[] heightmap;
}

void TerrainLayer::on_detach()
{
	
}

void TerrainLayer::on_update(TimeDelta time_delta)
{
	m_camera_controller.on_update(time_delta);

	Renderer2D::begin_scene(m_camera_controller.get_camera());
	Renderer2D::draw_tilemap(
		glm::vec2(0.0f),
		glm::vec2(m_map_size * 24, m_map_size * 9),
		m_atlas, m_tilemap, m_heightmap);
	Renderer2D::end_scene();
}

void TerrainLayer::on_event(Event& e)
{
	m_camera_controller.on_event(e);
}