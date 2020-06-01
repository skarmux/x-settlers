#include "AndroidWindow.h"

AndroidWindow::AndroidWindow(const WindowProps& props) {
	Initialize(props);
}

AndroidWindow::~AndroidWindow() {
	Shutdown();
}

void AndroidWindow::Initialize(const WindowProps& props) {
	m_Data.Title = "Android Window!"; //props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	/*if (!s_GLFWInitialized) {
		int success = glfwInit();
		CORE_ASSERT(success, "Could not initialize GLFW!");
		s_GLFWInitialized = true;
	}*/

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CORE_ERROR("Could not initialize SDL! SDL Error: %s", SDL_GetError());
	}

	//m_Window = glfwCreateWindow((int)props.Width,(int)props.Height,m_Data.Title.c_str(), nullptr, nullptr);
	
	m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_Data.Width, (int)m_Data.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	//m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_Data.Width, (int)m_Data.Height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

	//glfwMakeContextCurrent(m_Window);
	SDL_GLContext glContext = SDL_GL_CreateContext(m_Window);

	//glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);
}

void AndroidWindow::Shutdown() {
	//glfwDestroyWindow(m_Window);
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void AndroidWindow::OnUpdate() {
	//glfwPollEvents();
	//SDL_PollEvent();
	//glfwSwapBuffers(m_Window);
	SDL_GL_SwapWindow(m_Window);
}

void AndroidWindow::SetVSync(bool enabled) {
	if (enabled) {
		//glfwSwapInterval(1);
		SDL_GL_SetSwapInterval(1);
	}	
	else {
		//glfwSwapInterval(0);
		SDL_GL_SetSwapInterval(0);
	}

	m_Data.VSync = enabled;
}

bool AndroidWindow::IsVSync() const {
	return m_Data.VSync;
}
