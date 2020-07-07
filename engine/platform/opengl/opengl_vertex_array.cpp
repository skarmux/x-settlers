#include "platform/opengl/opengl_vertex_array.h"

#include <glad/glad.h>

static GLenum to_opengl_type(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float1:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
	case ShaderDataType::Mat3:
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int1:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}
	return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_renderer_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_renderer_id);
}

void OpenGLVertexArray::bind() const
{
	glBindVertexArray(m_renderer_id);
}

void OpenGLVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
{
	glBindVertexArray(m_renderer_id);
	
	vertex_buffer->bind();

	auto& layout = vertex_buffer->get_layout();
	for (const auto& element : layout)
	{
		switch (element.type)
		{
		case ShaderDataType::Float1:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Int1:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
		{
			glEnableVertexAttribArray(m_vertex_buffer_index);
			glVertexAttribPointer(
				m_vertex_buffer_index, // location
				element.get_component_count(), // number of components (vec3 = {float,float,float} = 3)
				to_opengl_type(element.type), // data type
				element.normalized ? GL_TRUE : GL_FALSE, // set normalized flag
				layout.get_stride(), // stride :: space between consecutive vertex attributes
				(const void*)element.offset // offset :: where the vertex data begins within the buffer
			);
			m_vertex_buffer_index++;
			break;
		}
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			uint8_t count = element.get_component_count();
			for (uint8_t i = 0; i < count; i++)
			{
				glEnableVertexAttribArray(m_vertex_buffer_index);
				glVertexAttribPointer(m_vertex_buffer_index,
					count,
					to_opengl_type(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.get_stride(),
					(const void*)(sizeof(float) * count * i));
				glVertexAttribDivisor(m_vertex_buffer_index, 1);
				m_vertex_buffer_index++;
			}
			break;
		}
		default:
			CORE_ERROR("Unknown ShaderDataType!");
		}
	}

	m_vertex_buffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_renderer_id);

	indexBuffer->bind();

	m_index_buffer = indexBuffer;
}