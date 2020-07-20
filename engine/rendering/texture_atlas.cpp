#include "rendering/texture_atlas.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void TextureAtlas::add_texture(const std::string& path, uint32_t& texture_id)
{
	int width, height, channels;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	RENDERER_INFO(
		"loaded texture {0} :\n\twidth = {1}, height = {2}, channels = {3}",
		path, width, height, channels);
	// 32-bit color -> Pixel [R:4B, G:4B, B:4B, A:4B] -> [16 Byte]
	// 32-bit color -> Pixel [R:4B, G:4B, B:4B]       -> [12 Byte]
	add_texture((uint32_t*)data, width, height, texture_id);
}

void TextureAtlas::add_texture(
	uint32_t* data, uint32_t width, uint32_t height, uint32_t& texture_id)
{	
	m_texture_spaces.emplace_back(Space{ 0, 0, width, height });
	m_texture_data.push_back(data);
	texture_id = m_texture_spaces.size() - 1;
}

bool TextureAtlas::insert_texture(
	std::vector<TextureAtlas::Space>& free_spaces,
	TextureAtlas::Space* texture)
{
	// assuming free_spaces are sorted from largest to smallest surface area

	for (int i = free_spaces.size() - 1; i >= 0; i--)
	{
		Space space = free_spaces[i];

		if (texture->width <= space.width && texture->height <= space.height)
		{
			// set texture position to be in the top-left corner of the free space

			texture->x = space.x;
			texture->y = space.y;

			// remove space entry from vector as it will be substituted

			free_spaces[i] = free_spaces.back();
			free_spaces.pop_back();
			
			if (texture->width < space.width && texture->height < space.height)
			{
				// split space in two

				if ((space.width - texture->width) > (space.height - texture->height))
				{
					// remaining width is bigger than remaining height
					// split space vertically first
					
					// append bigger split
					free_spaces.emplace_back(Space{
						space.x + texture->width, space.y,
						space.width - texture->width, space.height });

					// append smaller split
					free_spaces.emplace_back(Space{
						space.x, space.y + texture->height,
						texture->width, space.height - texture->height });
				}
				else
				{
					// remaining width is smaller or equal than remaining height
					// split space horizontally first
					
					// append bigger split
					free_spaces.emplace_back(Space{
						space.x, space.y + texture->height,
						space.width, space.height - texture->height });

					// append smaller split
					free_spaces.emplace_back(Space{
						space.x + texture->width, space.y,
						space.width - texture->width, texture->height });
				}
			}
			else if (texture->width == space.width && texture->height == space.height)
			{
				// do not create any substitutes
				return true;
			}
			else if (texture->width == space.width)
			{
				// create horizontal split
				free_spaces.emplace_back(Space{
					space.x, space.y + texture->height,
					space.width, space.height - texture->height });
			}
			else if (texture->height == space.height)
			{
				// create vertical split
				free_spaces.emplace_back(Space{
					space.x + texture->width, space.y,
					space.width - texture->width, space.height });
			}
			return true;
		}
	}
	return false;
}

std::shared_ptr<Texture2D> TextureAtlas::create_atlas_texture(
	uint32_t max_width, uint32_t max_height)
{
	uint32_t atlas_width = max_width;
	uint32_t atlas_height = max_height;

	std::vector<Space> free_spaces;
	free_spaces.emplace_back(Space{ 0, 0, atlas_width, atlas_height });

	std::vector<Space*> texture_space_ptrs(m_texture_spaces.size());
	for (uint32_t i = 0; i < m_texture_spaces.size(); i++)
		texture_space_ptrs[i] = &m_texture_spaces[i];

	// area
	std::sort(
		texture_space_ptrs.begin(),
		texture_space_ptrs.end(),
		[](Space* a, Space* b) { 
			return (a->width * a->height) > (b->width * b->height); 
		});

	//// perimeter
	//std::sort(texture_space_ptrs.begin(), texture_space_ptrs.end(),
	//	[](Space* a, Space* b) { return (a->width + a->height) * 2 > (b->width + b->height) * 2; });

	//// the bigger side
	//std::sort(texture_space_ptrs.begin(), texture_space_ptrs.end(),
	//	[](Space* a, Space* b) { return std::max(a->width,a->height) > std::max(b->width,b->height); });

	//// width
	//std::sort(texture_space_ptrs.begin(), texture_space_ptrs.end(),
	//	[](Space* a, Space* b) { return a->width > b->width; });

	//// height
	//std::sort(texture_space_ptrs.begin(), texture_space_ptrs.end(),
	//	[](Space* a, Space* b) { return a->height > b->height; });

	//// max(w,h) / min(w,h) * w * h
	//std::sort(texture_space_ptrs.begin(), texture_space_ptrs.end(),
	//	[](Space* a, Space* b) { return 
	//		(std::max(a->width, a->height) / std::min(a->width, a->height) * a->width * a->height) > 
	//		(std::max(b->width, b->height) / std::min(b->width, b->height) * b->width * b->height); });

	for (uint32_t i = 0; i < texture_space_ptrs.size(); i++)
	{
		bool success = insert_texture(free_spaces, texture_space_ptrs[i] );
		if (!success) {
			RENDERER_WARN("texture couldn't fit into atlas!");
			free_spaces.clear();
			break;
		}
	}
	
	uint32_t atlas_channels = 4; // RGBA 4*32b
	uint32_t* atlas_data = new uint32_t[atlas_width * atlas_height * atlas_channels];

	for (uint32_t i = 0; i < m_texture_spaces.size(); i++)
	{
		TextureAtlas::Space &texture_space = m_texture_spaces[i];
		uint32_t* &texture_data = m_texture_data[i];

		for (uint32_t row = 0; row < texture_space.height; row++)
		{
			uint32_t atlas_data_offset = 
				(atlas_width * (texture_space.y + row)) + texture_space.x;
			uint32_t texture_data_offset = texture_space.width * row;
			memcpy(
				atlas_data + atlas_data_offset, 
				texture_data + texture_data_offset, 
				texture_space.width * atlas_channels);
		}
		delete[] texture_data;
	}
	m_texture_data.clear();

	std::shared_ptr<Texture2D> texture = Texture2D::create(atlas_width, atlas_height);
	texture->push_data(
		atlas_data, atlas_width * atlas_height * atlas_channels * sizeof(uint32_t));

	stbi_write_png("C:/Users/Nils/Desktop/atlas.png",
		atlas_width, atlas_height, 4, atlas_data, atlas_width * 4);

	delete[] atlas_data;

	return texture;
}