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

	void push_data(void* data, size_t size) override;

	void bind(uint32_t slot = 0) const override;
private:
	uint32_t m_width, m_height, m_channels, m_bit_depth;
	uint32_t m_renderer_id;
	GLenum m_internal_format, m_data_format;
};

class OpenGLTexture3D : public Texture3D
{
public:
	OpenGLTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t channels, uint32_t bit_depth);
	OpenGLTexture3D(const std::vector<std::string>& paths);
	~OpenGLTexture3D();

	uint32_t get_width() const override { return m_width; }
	uint32_t get_height() const override { return m_height; }
	uint32_t get_depth() const override { return m_depth; }

	void push_data(void* data, size_t size) override;

	void bind(uint32_t slot = 0) const override;
private:
	uint32_t m_width, m_height, m_depth, m_channels, m_bit_depth;
	uint32_t m_renderer_id;
	GLenum m_internal_format, m_data_format;
};

//class OpenGLBufferTexture : public BufferTexture
//{
//public:
//	OpenGLBufferTexture(uint32_t size);
//	~OpenGLBufferTexture();
//
//	uint32_t get_size() const override { return m_size; }
//
//	void set_data(void* data) override;
//
//	void bind(uint32_t slot = 0) const override;
//private:
//	uint32_t m_size;
//	uint32_t m_tbo_renderer_id;
//	uint32_t m_tbo_tex_renderer_id;
//	GLenum m_internal_format, m_data_format;
//};