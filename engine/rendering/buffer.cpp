#include "rendering/buffer.h"
#include "rendering/renderer_api.h"

#include "platform/opengl/opengl_buffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
{
	switch (RendererAPI::get_api()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(size);
	default: return nullptr;
	}
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
{
	switch (RendererAPI::get_api()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	default: return nullptr;
	}
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t* vertices, uint32_t size)
{
	switch (RendererAPI::get_api()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLIndexBuffer>(vertices, size);
	default: return nullptr;
	}
}
