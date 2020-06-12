#include "opengl_context.h"
#include "core/log.h"

GLESContext::OpenGLContext(SDL_Window* window) :
	m_WindowHandle(window),
	m_GLContext(SDL_GL_CreateContext(window)) {}

void GLESContext::Init()
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

	// initialize glad
	/*if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		CORE_ERROR("Failed to initialize glad!");
		exit(2);
	}*/

	/*int w, h;
	SDL_GetWindowSize(m_WindowHandle, &w, &h);
	RenderCommand::SetViewport(0, 0, w, h);*/
}

void GLESContext::SwapBuffers()
{
	//SDL_GL_SwapWindow(m_WindowHandle);
}

void GLESContext::SetSwapInterval(int interval) {
	//SDL_GL_SetSwapInterval(interval);
}