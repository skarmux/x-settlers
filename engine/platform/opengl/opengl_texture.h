#pragma once

#include "rendering/texture.h"

#include <glad/glad.h>

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height, 
		Texture::Format comp, Texture::Type type);
	~OpenGLTexture2D() { glDeleteTextures(1, &m_renderer_id); }

	uint32_t width() const override { return m_width; }
	uint32_t height() const override { return m_height; }

	void push_data(uint8_t* data,
		uint32_t width, uint32_t height,
		Texture::Format comp, Texture::Type type) override;

	void push_data(uint8_t* data,
		uint32_t x, uint32_t y,
		uint32_t width, uint32_t height,
		Texture::Format comp, Texture::Type type) override;

	void generate_mipmaps() override { glGenerateMipmap(GL_TEXTURE_2D); };

	void bind(uint32_t slot = 0) const override;

	void set_filter(Filter filter) override;
	void set_wrapping(Wrap wrap) override;
private:
	GLenum m_renderer_id;
	uint32_t m_width, m_height;
	Texture::Format m_comp;
	Texture::Type m_type;
};
