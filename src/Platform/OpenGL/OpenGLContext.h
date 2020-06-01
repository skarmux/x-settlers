#pragma once

#include "Renderer/GraphicsContext.h"

#include <SDL.h>

class OpenGLContext : public GraphicsContext
{
public:
	OpenGLContext(SDL_Window* windowHandle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
	virtual void SetSwapInterval(int interval) override;

private:
	SDL_Window* m_WindowHandle;
	SDL_GLContext m_GLContext;
};