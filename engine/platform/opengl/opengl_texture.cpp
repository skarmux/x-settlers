#include "platform/opengl/opengl_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	: m_width(width), m_height(height)
{
	m_internal_format = GL_RGBA8;
	m_data_format = GL_RGBA;

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

	/*glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);*/

	// set texture filtering parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// set texture wrapping parameters
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	int width, height, channels;
	//stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	m_width = width;
	m_height = height;

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		RENDERER_INFO("Texture loaded: {0}", path);
	}
	else {
		RENDERER_ERROR("Failed to load texture: {0}", path);
	}

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_renderer_id);
}

void OpenGLTexture2D::set_data(void* data, uint32_t size)
{
	uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
	glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
	//glBindTextureUnit(slot, m_RendererID);
}