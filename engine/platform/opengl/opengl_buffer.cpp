#include "opengl_buffer.h"

//#include <glad/glad.h>

/*=================================================================*\
|| VERTEX BUFFER                                                   ||
\*=================================================================*/

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
	/*glCreateBuffers(1, &m_RendererID);
	glBindBuffers(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);*/
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
	/*glCreateBuffers(1, &m_RendererID);
	glBindBuffers(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);*/
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	//glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*=================================================================*\
|| INDEX BUFFER                                                    ||
\*=================================================================*/

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
	: m_Count(count)
{
	//glCreateBuffers(1, &m_RendererID);

	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	//glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const
{
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}