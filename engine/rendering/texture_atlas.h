#pragma once

#include "rendering/texture.h"

#include <glm/vec2.hpp>

/* Stores and manages raw image data and creates a Texture2D when finished. */
class TextureAtlas
{
public:
	TextureAtlas() : m_width(0), m_height(0) {}
	void translate_to_atlas_coords(uint32_t tex_id, glm::vec2& tex_coord) const;

	void add_texture(const std::string& path);
	void add_texture(uint32_t* data, uint32_t width, uint32_t height);

	std::shared_ptr<Texture2D> create_texture(uint32_t width, uint32_t height);
private:
	struct Space
	{
		uint32_t x, y;
		uint32_t width, height;
	};
	bool insert_texture(std::vector<Space>& free_spaces, Space* texture);
private:
	uint32_t m_width, m_height;
	std::vector<Space> m_texture_spaces;
	std::vector<uint32_t*> m_texture_data;
};