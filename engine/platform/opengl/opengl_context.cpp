#include "platform/opengl/opengl_context.h"

OpenGLContext::OpenGLContext(SDL_Window* window) :
	m_window_handle(window),
	m_gl_context(SDL_GL_CreateContext(window)) {}

void OpenGLContext::init()
{
	// don't load additional libraries
	SDL_GL_LoadLibrary(NULL);
	// request OpenGL 4.6 context
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// request depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// create OpenGL context and make it current
	SDL_GL_MakeCurrent(m_window_handle, m_gl_context);
}

void OpenGLContext::swap_buffers()
{
	SDL_GL_SwapWindow(m_window_handle);
}

void OpenGLContext::set_swap_interval(int interval) {
	SDL_GL_SetSwapInterval(interval);
}