#include "platform/opengl/opengl_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	: m_width(width), m_height(height), m_internal_format(GL_RGBA8), m_data_format(GL_RGBA)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
	glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
{
	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	// set texture filtering parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// set texture wrapping parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), (int*)&m_width, (int*)&m_height, (int*)&m_channels, 0);

	if (data) {
		switch (m_channels)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D); // TODO: let the user decide over mipmapping
		RENDERER_INFO("Texture loaded: {0}; {1} channel(s); {2} width; {3} height", path, m_channels, m_width, m_height);
	}
	else {
		RENDERER_ERROR("Failed to load texture: {0}", path);
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_renderer_id);
}

void OpenGLTexture2D::push_data(void* data, size_t size)
{
	glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

OpenGLTexture3D::OpenGLTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t channels, uint32_t bit_depth)
	: m_width(width), m_height(height), m_channels(channels), m_bit_depth(bit_depth)
{
	/*switch (channels)
	{
	case 1:
		m_internal_format = GL_R;
		break;
	case 3:
		m_internal_format = GL_RGB;
		break;
	case 4:
		m_internal_format = GL_RGBA;
		break;
	default:
		m_internal_format = GL_RGBA;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
	glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
}

OpenGLTexture3D::OpenGLTexture3D(const std::vector<std::string>& paths)
	: m_depth(paths.size())
{
	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_3D, m_renderer_id);

	// set texture filtering parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// set texture wrapping parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_R, GL_REPEAT);

	stbi_uc* data = stbi_load(paths[0].c_str(), (int*)&m_width, (int*)&m_height, (int*)&m_channels, 0);

	if (data) {
		switch (m_channels)
		{
		case 1:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, m_width, m_height, m_depth, 0, GL_R, GL_UNSIGNED_BYTE, nullptr);
			break;
		case 3:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, m_width, m_height, m_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			break;
		case 4:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, m_width, m_height, m_depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			break;
		}
		RENDERER_INFO("Texture loaded: {0}; {1} channel(s); {2} width; {3} height", paths[0], m_channels, m_width, m_height);
	}
	else {
		RENDERER_ERROR("Failed to load texture: {0}", paths[0]);
	}

	stbi_image_free(data);

	for (uint32_t i = 1; i < m_depth; i++)
	{
		data = stbi_load(paths[i].c_str(), (int*)&m_width, (int*)&m_height, (int*)&m_channels, 0);

		glTextureSubImage3D(m_renderer_id, 0, 0, i, 0, m_width, m_height, m_depth, m_data_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}

OpenGLTexture3D::~OpenGLTexture3D()
{
	glDeleteTextures(1, &m_renderer_id);
}

void OpenGLTexture3D::push_data(void* data, size_t size)
{
	glTextureSubImage3D(m_renderer_id, 0, 0, 0, 0, m_width, m_height, m_depth, m_data_format, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture3D::bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_3D, m_renderer_id);
}

//OpenGLBufferTexture::OpenGLBufferTexture(uint32_t size)
//	: m_size(size)
//{
//	m_internal_format = GL_R8UI; // 1 Channel (R) ; 8 bit (1 Byte) ; U-nsigned I-nteger
//	m_data_format = GL_R8UI;
//
//	glGenBuffers(1, &m_tbo_renderer_id);
//	glBindBuffer(GL_TEXTURE_BUFFER, m_tbo_renderer_id);
//
//	uint8_t* data = new uint8_t[size];
//	for (int i = 0; i < size; i++)
//		data[i] = 10;
//
//	glBufferData(GL_TEXTURE_BUFFER, size * sizeof(uint8_t), data, GL_STATIC_DRAW);
//
//	glGenTextures(1, &m_tbo_tex_renderer_id);
//
//	glBindBuffer(GL_TEXTURE_BUFFER, 0);
//}
//
//OpenGLBufferTexture::~OpenGLBufferTexture()
//{
//	glDeleteTextures(1, &m_tbo_tex_renderer_id);
//	glDeleteBuffers(1, &m_tbo_renderer_id);
//}
//
//void OpenGLBufferTexture::set_data(void* data)
//{
//	glBindBuffer(GL_TEXTURE_BUFFER, m_tbo_renderer_id);
//	glBufferSubData(GL_TEXTURE_BUFFER, 0, m_size * sizeof(uint8_t), data);
//}
//
//void OpenGLBufferTexture::bind(uint32_t slot) const
//{
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_BUFFER, m_tbo_tex_renderer_id);
//	glTexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, m_tbo_renderer_id);
//	glUniform1i(m_tbo_tex_renderer_id, 0);
//}