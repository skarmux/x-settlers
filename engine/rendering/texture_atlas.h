#pragma once

#include "rendering/texture.h"

#include <glm/vec2.hpp>

/* Stores and manages raw image data and creates a Texture2D when finished. */
class TextureAtlas
{
public:
	struct Space
	{
		uint32_t x, y;
		uint32_t width, height;
	};
public:
	TextureAtlas::Space get_texture_space(const uint32_t texture_id) const { return m_texture_spaces[texture_id]; };

	void add_texture(const std::string& path, uint32_t& texture_id);
	void add_texture(uint32_t* data, uint32_t width, uint32_t height, uint32_t& texture_id);

	std::shared_ptr<Texture2D> create_atlas_texture(uint32_t width, uint32_t height);
private:
	bool insert_texture(std::vector<Space>& free_spaces, Space* texture);
private:
	std::vector<Space> m_texture_spaces;
	std::vector<uint32_t*> m_texture_data;
};