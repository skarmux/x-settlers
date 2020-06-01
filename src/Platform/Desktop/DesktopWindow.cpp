#include "DesktopWindow.h"
#include "Renderer/Renderer2D.h"

DesktopWindow::DesktopWindow(const WindowProps& props) {
	Init(props);
}

DesktopWindow::~DesktopWindow() {
	Shutdown();
}

void DesktopWindow::Init(const WindowProps& props) {
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CORE_ERROR("Could not initialize SDL! SDL Error: %s", SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);

	// create the window
	if (Renderer2D::GetAPI() == RendererAPI::API::OpenGL) {
		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_Data.Width, (int)m_Data.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	}
	else if (Renderer2D::GetAPI() == RendererAPI::API::Vulkan) {
		//m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_Data.Width, (int)m_Data.Height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
	}

	if (m_Window == nullptr) {
		CORE_ERROR("Couldn't set video mode!");
		exit(2);
	}

	m_Context = GraphicsContext::Create(m_Window);
	m_Context->Init();

	//glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true); // default setting

	// TODO: Set SDL callbacks
	// ...
}

void DesktopWindow::Shutdown() {
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void DesktopWindow::OnUpdate() {
	//SDL_PollEvent();
	m_Context->SwapBuffers();
}

void DesktopWindow::SetVSync(bool enabled) {
	if (enabled) {
		m_Context->SetSwapInterval(1);
	}	
	else {
		m_Context->SetSwapInterval(0);
	}

	m_Data.VSync = enabled;
}

bool DesktopWindow::IsVSync() const {
	return m_Data.VSync;
}
