#include "rendering/texture_atlas.h"
#include "platform/filesystem.h"

void TextureAtlas::add_image(uint8_t* data, uint32_t width, uint32_t height, Texture::Format format)
{
	if (format != m_format)
	{
		//TODO
		CORE_ERROR("channel translation not implemented yet!");
	}

	m_image_spaces.emplace_back( Space{ 0, 0, width, height, data } );
}

void TextureAtlas::add_image(const std::string& path)
{
	uint32_t atlas_channels = static_cast<uint32_t>(m_format);

	uint32_t width, height, channels;
	uint8_t* data_ptr = Filesystem::image_load(path.c_str(), &width, &height, &channels, atlas_channels);

	uint8_t* new_data_ptr = new uint8_t[(uint64_t)width * (uint64_t)height * (uint64_t)atlas_channels];

	memcpy(new_data_ptr, data_ptr, (uint64_t)width * (uint64_t)height * (uint64_t)atlas_channels * sizeof(uint8_t));

	Filesystem::image_free(data_ptr);

	m_image_spaces.emplace_back(Space{ 0, 0, width, height, new_data_ptr });
}

bool TextureAtlas::insert_image(
	std::vector<TextureAtlas::Space>& free_spaces,
	TextureAtlas::Space* img_space)
{
	// assuming free_spaces are sorted from largest to smallest surface area

	for (int i = free_spaces.size() - 1; i >= 0; i--)
	{
		Space free_space = free_spaces[i];

		if (img_space->width <= free_space.width && img_space->height <= free_space.height)
		{
			// set texture position to be in the top-left corner of the free space

			img_space->x = free_space.x;
			img_space->y = free_space.y;

			// remove space entry from vector as it will be substituted

			free_spaces[i] = free_spaces.back();
			free_spaces.pop_back();
			
			if (img_space->width < free_space.width && img_space->height < free_space.height)
			{
				// split space in two

				if ((free_space.width - img_space->width) > (free_space.height - img_space->height))
				{
					// remaining width is bigger than remaining height
					// split space vertically first
					
					// append bigger split
					free_spaces.emplace_back(Space{
						free_space.x + img_space->width,
						free_space.y,
						free_space.width - img_space->width,
						free_space.height });

					// append smaller split
					free_spaces.emplace_back(Space{
						free_space.x, 
						free_space.y + img_space->height,
						img_space->width,
						free_space.height - img_space->height });
				}
				else
				{
					// remaining width is smaller or equal than remaining height
					// split space horizontally first
					
					// append bigger split
					free_spaces.emplace_back(Space{
						free_space.x, 
						free_space.y + img_space->height,
						free_space.width, 
						free_space.height - img_space->height });

					// append smaller split
					free_spaces.emplace_back(Space{
						free_space.x + img_space->width,
						free_space.y,
						free_space.width - img_space->width,
						img_space->height });
				}
			}
			else if (img_space->width == free_space.width && img_space->height == free_space.height)
			{
				// do not create any substitutes
				return true;
			}
			else if (img_space->width == free_space.width)
			{
				// create horizontal split
				free_spaces.emplace_back(Space{
					free_space.x, 
					free_space.y + img_space->height,
					free_space.width, 
					free_space.height - img_space->height });
			}
			else if (img_space->height == free_space.height)
			{
				// create vertical split
				free_spaces.emplace_back(Space{
					free_space.x + img_space->width, free_space.y,
					free_space.width - img_space->width, free_space.height });
			}
			return true;
		}
	}
	return false;
}

std::shared_ptr<Texture2D> TextureAtlas::create_texture(
	uint32_t max_width, uint32_t max_height)
{
	std::vector<Space> free_spaces;
	free_spaces.emplace_back(Space{ 0, 0, max_width, max_height });

	std::vector<Space*> image_sorting(m_image_spaces.size());

	// fill to-be-sorted vector with image_space references
	for (uint32_t i = 0; i < m_image_spaces.size(); i++)
		image_sorting[i] = &m_image_spaces[i];

	// sort image_spaces by...

	// area
	std::sort(image_sorting.begin(), image_sorting.end(),
		[](Space* a, Space* b) { 
			return (a->width * a->height) > (b->width * b->height); 
		} );

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

	// try inserting all available images into available free space
	for (uint32_t i = 0; i < image_sorting.size(); i++)
	{
		bool success = insert_image(free_spaces, image_sorting[i] );
		if (!success) {
			RENDERER_WARN("texture couldn't fit into atlas!");
			free_spaces.clear();
			image_sorting.clear();
			break;
		}
	}

	uint32_t atlas_channels = static_cast<uint32_t>(m_format);

	std::shared_ptr<Texture2D> texture = Texture2D::create(max_width, max_height, m_format);

	//uint8_t* test = new uint8_t[max_width * max_height * atlas_channels];

	// start pushing data to texture
	for (uint32_t i = 0; i < m_image_spaces.size(); i++)
	{
		TextureAtlas::Space& space = m_image_spaces[i];
		texture->push_data(space.data, space.x, space.y, space.width, space.height, m_format, Texture::Type::UNSIGNED_BYTE);
		/*uint8_t* pos = test + (space.y * max_width * atlas_channels) + (space.x * atlas_channels);
		for (uint32_t row = 0; row < space.height; row++)
		{
			memcpy(pos + (row * max_width * atlas_channels), space.data + (row * space.width * atlas_channels),
				space.width * atlas_channels * sizeof(uint8_t));
		}*/
		delete[] space.data;
	}

	/*Filesystem::write_png("C:\\Users\\Nils\\Desktop\\my_atlas.png", 
		max_width, max_height, atlas_channels,
		test );

	delete[] test;*/

	return texture;
}