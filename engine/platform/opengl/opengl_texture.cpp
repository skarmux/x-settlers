#include "platform/opengl/opengl_texture.h"

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, 
	Format comp, Type type)
	: m_width(width), m_height(height), m_comp(comp), m_type(type)
{
	glGenTextures(1, &m_renderer_id);

	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	GLenum gl_format;
	switch (m_comp) {
	case Format::RED:
		if (type == Type::UNSIGNED_BYTE) gl_format = GL_R8;
		if (type == Type::UNSIGNED_INT) gl_format = GL_R8UI;
		break;
	case Format::RG: 
		gl_format = GL_RG8; break;
	case Format::RGB: 
		gl_format = GL_RGB8; break;
	case Format::RGBA:
		if (type == Type::UNSIGNED_BYTE) gl_format = GL_RGBA8;
		if (type == Type::UNSIGNED_INT) gl_format = GL_RGBA8UI;
		break;
	}

	glTextureStorage2D(m_renderer_id, 1, gl_format, m_width, m_height);

	set_filter(Filter::NEAREST);
	set_wrapping(Wrap::CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0); // optional line
}

void OpenGLTexture2D::push_data(uint8_t* data, 
	uint32_t width, uint32_t height, 
	Format comp, Type type)
{
	push_data(data, 0, 0, width, height, comp, type);
}

void OpenGLTexture2D::push_data(uint8_t* data, 
	uint32_t x, uint32_t y, 
	uint32_t width, uint32_t height, 
	Format format, Type type)
{
	GLenum gl_format, gl_type = GL_UNSIGNED_BYTE;
	switch (m_comp) {
	case Format::RED:
		if (type == Type::UNSIGNED_BYTE) { gl_format = GL_RED; }
		if (type == Type::UNSIGNED_INT) { gl_format = GL_RED_INTEGER; }
		break;
	case Format::RG: gl_format = GL_RG; break;
	case Format::RGB: gl_format = GL_RGB;	break;
	case Format::RGBA:
		if (type == Type::UNSIGNED_BYTE) { gl_format = GL_RGBA; }
		if (type == Type::UNSIGNED_INT) { gl_format = GL_RGBA_INTEGER; }
		break;
	}

	// http://docs.gl/gl4/glTexSubImage2D
	// void glTexSubImage2D(GLenum target​, GLint level​, GLint xoffset​, GLint yoffset​, GLsizei width​, GLsizei height​, GLenum format​, GLenum type​, const GLvoid * data​);
	glTextureSubImage2D(m_renderer_id, 0, x, y, width, height, gl_format, gl_type, data);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void OpenGLTexture2D::set_filter(Filter filter)
{
	/* If the internalformat is integer, TEXTURE_MAG_FILTER must be
	   NEAREST and TEXTURE_MIN_FILTER must be NEAREST or
	   NEAREST_MIPMAP_NEAREST */
	GLenum gl_filter;
	switch (filter) {
	case Filter::NEAREST: gl_filter = GL_NEAREST; break;
	case Filter::LINEAR: gl_filter = GL_LINEAR; break;
	}
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, gl_filter);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, gl_filter);
}

void OpenGLTexture2D::set_wrapping(Wrap wrap)
{
	GLenum gl_wrap;
	switch (wrap) {
	case Wrap::CLAMP: gl_wrap = GL_CLAMP; break;
	case Wrap::REPEAT: gl_wrap = GL_REPEAT; break;
	}
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, gl_wrap);
	glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, gl_wrap);
}