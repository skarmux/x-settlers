#pragma once

#include "rendering/texture.h"

#include <glm/vec2.hpp>

/* Stores and manages raw image data and creates a Texture2D when finished. */
class TextureAtlas
{
public:
	TextureAtlas(Texture::Format comp) : m_format(comp) {}
	~TextureAtlas()	{ m_image_spaces.clear(); }

	void add_image(const std::string& path);
	void add_image(uint8_t* data, uint32_t width, uint32_t height, Texture::Format format);

	glm::uvec2 texel_offset(uint32_t tex_id) const
	{ 
		return glm::uvec2(m_image_spaces[tex_id].x, m_image_spaces[tex_id].y); 
	}

	std::shared_ptr<Texture2D> create_texture(uint32_t max_width, uint32_t max_height);
private:
	struct Space
	{
		uint32_t x, y;
		uint32_t width, height;
		uint8_t* data;
	};
	bool insert_image(std::vector<Space>& free_spaces, Space* texture);
private:
	Texture::Format m_format;
	std::vector<Space> m_image_spaces;
};