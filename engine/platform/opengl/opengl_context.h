#pragma once

#include "rendering/graphics_context.h"

#include <SDL.h> // TODO remove SDL dependency

class OpenGLContext : public GraphicsContext
{
public:
	OpenGLContext(SDL_Window* windowHandle);

	void init() override;
	void swap_buffers() override;
	void set_swap_interval(int interval) override;
private:
	SDL_Window* m_window_handle;
	SDL_GLContext m_gl_context;
};