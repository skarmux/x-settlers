#include "buffer.h"

#include "renderer_api.h"
#include "../platform/opengl/opengl_buffer.h"
//#include "../platform/vulkan/vulkan_buffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(size);
	case RendererAPI::API::Vulkan:	break; // return new VulkanVertexBuffer(vertices, size);
	}

	return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	case RendererAPI::API::Vulkan:	break; // return new VulkanVertexBuffer(vertices, size);
	}

	return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* vertices, uint32_t size)
{
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLIndexBuffer>(vertices, size);
	case RendererAPI::API::Vulkan:	return nullptr; //return new VulkanIndexBuffer(vertices, size);
	default: return nullptr;
	}
}
