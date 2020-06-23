#include "renderer_api.h"

#include "platform/opengl/opengl_renderer_api.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::Create()
{
	switch (s_API) {
		case RendererAPI::API::OpenGL:	return std::make_unique<OpenGLRendererAPI>();
		case RendererAPI::API::Vulkan:	return nullptr;
		default: return nullptr;
	}
}
