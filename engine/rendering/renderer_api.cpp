#include "rendering/renderer_api.h"

#include "platform/opengl/opengl_renderer_api.h"

RendererAPI::API RendererAPI::s_api = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::create()
{
	switch (s_api) {
		case RendererAPI::API::OpenGL:	return std::make_unique<OpenGLRendererAPI>();
		default: return nullptr;
	}
}
