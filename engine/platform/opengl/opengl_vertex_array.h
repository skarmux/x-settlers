#pragma once

#include "rendering/vertex_array.h"

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const { return m_vertex_buffers; }
	virtual const std::shared_ptr<IndexBuffer>& get_index_buffer() const { return m_index_buffer; }
private:
	uint32_t m_renderer_id;
	uint32_t m_vertex_buffer_index = 0;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
	std::shared_ptr<IndexBuffer> m_index_buffer;
};