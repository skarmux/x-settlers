#pragma once

#include "rendering/texture.h"

#include <glad/glad.h>

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height);
	OpenGLTexture2D(const std::string& path);
	~OpenGLTexture2D();

	uint32_t get_width() const override { return m_width; }
	uint32_t get_height() const override { return m_height; }

	void set_data(void* data, uint32_t size) override;

	void bind(uint32_t slot = 0) const override;
private:
	uint32_t m_width, m_height;
	uint32_t m_renderer_id;
	GLenum m_internal_format, m_data_format;
};