#include "opengl_context.h"
#include "core/log.h"

OpenGLContext::OpenGLContext(SDL_Window* window) :
	m_WindowHandle(window),
	m_GLContext(SDL_GL_CreateContext(window)) {}

void OpenGLContext::Init()
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
	SDL_GL_MakeCurrent(m_WindowHandle, m_GLContext);
}

void OpenGLContext::SwapBuffers()
{
	SDL_GL_SwapWindow(m_WindowHandle);
}

void OpenGLContext::SetSwapInterval(int interval) {
	SDL_GL_SetSwapInterval(interval);
}