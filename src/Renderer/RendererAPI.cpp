#include "RendererAPI.h"

#include "../Platform/OpenGL/OpenGLRendererAPI.h"

#if (RENDERER == Vulkan)
RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#elif (RENDERER == OpenGL)
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#else
#error Unknown renderer!
#endif

std::unique_ptr<RendererAPI> RendererAPI::Create()
{
	switch (s_API) {
	case RendererAPI::API::OpenGL:	return std::make_unique<OpenGLRendererAPI>();
	case RendererAPI::API::Vulkan:	return nullptr;
	default: return nullptr;
	}
}
