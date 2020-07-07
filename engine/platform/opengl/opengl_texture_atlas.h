#pragma once

#include "rendering/texture_atlas.h"

#include <glad/glad.h>

class OpenGLTextureAtlas : public TextureAtlas
{
public:
	struct TextureAtlasPos
	{
		int x;
		int y;
		int width;
		int height;
	};
private:
	enum class TextureSizeSlot
	{
		Height8, Height16, Height32, Height64, Height85, Height105, Height128, Height256, COUNT
	};
public:
	OpenGLTextureAtlas(uint32_t width, uint32_t height);
	~OpenGLTextureAtlas();

	uint32_t get_width() const override { return m_width; };
	uint32_t get_height() const override { return m_height; };

	void set_data(void* data, uint32_t size) override;

	void bind(uint32_t slot = 0) const override;

	void add_texture(const Texture& texture) override;
	//void add_texture(TextureAtlasPos* dst_pos, uint32_t* data, uint32_t width, uint32_t height, uint32_t repeat_width = 0) override;
private:
	TextureSizeSlot get_slot_by_height(uint32_t height);

	void copy_image(uint32_t* dst, uint32_t* src,
		uint32_t pos_x, uint32_t pos_y,
		uint32_t width, uint32_t height,
		uint32_t dst_width, uint32_t src_width);
private:
	uint32_t m_width, m_height;
	uint32_t* m_slot_pos_x;
	uint32_t* m_slot_pos_y;

	uint32_t m_filled_y_pos;
	uint32_t m_fill_state;

	uint32_t* m_img_buffer;

	uint32_t m_renderer_id;
	//GLenum m_internal_format, m_data_format;
};