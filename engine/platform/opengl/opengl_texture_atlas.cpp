#include "platform/opengl/opengl_texture_atlas.h"

static const int SLOT_HEIGHT[] = { 8, 16, 32, 64, 85, 105, 128, 256 };

OpenGLTextureAtlas::OpenGLTextureAtlas(uint32_t width, uint32_t height)
	: m_width(width), m_height(height), m_fill_state(0), m_filled_y_pos(0)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
	
	m_img_buffer = new uint32_t[(uint64_t)width * (uint64_t)height];

	m_slot_pos_x = new uint32_t[(int)TextureSizeSlot::COUNT];
	m_slot_pos_y = new uint32_t[(int)TextureSizeSlot::COUNT];

	for (int i = 0; i < (int)TextureSizeSlot::COUNT; i++)
	{
		m_slot_pos_x[i] = m_width; // slot is full
		m_slot_pos_y[i] = 0;
	}
}

OpenGLTextureAtlas::~OpenGLTextureAtlas()
{
	delete[] m_img_buffer;

	if (m_renderer_id != NULL)
		glDeleteTextures(1, &m_renderer_id);
}

OpenGLTextureAtlas::TextureSizeSlot OpenGLTextureAtlas::get_slot_by_height(uint32_t height)
{
	for (int i = 0; i < (int)TextureSizeSlot::COUNT; i++)
	{
		if (height <= SLOT_HEIGHT[i])
			return static_cast<TextureSizeSlot>(i);
	}

	return TextureSizeSlot::COUNT;
}

void OpenGLTextureAtlas::copy_image(uint32_t* dest, uint32_t* src,
	uint32_t pos_x, uint32_t pos_y,
	uint32_t width, uint32_t height,
	uint32_t dest_width, uint32_t src_width)
{
	if (src_width < 0)
		src_width = width;

	uint32_t* dest_p = dest + dest_width * pos_y;

	for (uint32_t y = 0; y < height; y++)
	{
		uint32_t* cur_dest_p = dest_p + dest_width * y + pos_x;
		uint32_t* srcP = src + (height - y - 1) * src_width;

		for (uint32_t x = width; x > 0; x--)
		{
			*cur_dest_p = *srcP;
			cur_dest_p++;
			srcP++;
		}
	}
}

void OpenGLTextureAtlas::set_data(void* data, uint32_t size)
{
	/*uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
	glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);*/
}

void OpenGLTextureAtlas::bind(uint32_t slot) const
{
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, m_img_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[] m_img_buffer;
}

void OpenGLTextureAtlas::add_texture(const Texture& texture)
{

}
//void OpenGLTextureAtlas::add_texture(TextureAtlasPos* dst_pos, uint32_t* data, uint32_t width, uint32_t height, uint32_t repeat_width)
//{
//
//}
