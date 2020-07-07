#include "rendering/graphics_context.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_context.h"

#include <SDL.h> // TODO: remove SDL dependency

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
{
	switch (Renderer2D::get_api()) {
	case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLContext>(static_cast<SDL_Window*>(window));
	default: return nullptr;
	}
}
