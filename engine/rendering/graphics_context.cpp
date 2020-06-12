#include "graphics_context.h"

#include "renderer_2d.h"
#include "platform/opengl/opengl_context.h"

#include <SDL.h>

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
{
	switch (Renderer2D::GetAPI()) {
	case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLContext>(static_cast<SDL_Window*>(window));
	case RendererAPI::API::Vulkan:  return nullptr; // return std::make_unique<VulkanContext>(static_cast<SDL_Window*>(window));
	}

	return nullptr;
}
