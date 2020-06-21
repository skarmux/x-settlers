#include "core/application.h"
#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "core/log.h"
#include <SDL.h>

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;

	m_Window = Window::Create();
	
	RenderCommand::Init();

	Renderer2D::Init();
}

Application::~Application()
{
	Renderer2D::Shutdown();
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

void Application::Run()
{
	SDL_Event e;

	while (m_Running)
	{
		// event handling
		while (SDL_PollEvent(&e) != 0)
		{
			// Application events
			if (e.type == SDL_QUIT) {
				m_Running = false;
			}

			// Window events
			else if (e.type == SDL_WINDOWEVENT) {
				switch (e.type) {
				case SDL_WINDOWEVENT_RESIZED: break;
				}
			}
			else if (e.type == SDL_SYSWMEVENT) {

			}
			
			switch (e.type) {
			// Keyboard Events
				// TODO
			// Mouse Events
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEWHEEL:
				break;
			// Touch Events
				// TODO
			// Gesture Events
				// TODO
			}

			{
				// update layer stack
				for (Layer* layer : m_LayerStack)
					layer->OnEvent(e);
			}
		}
		
		float time = 0.16; //(float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			{
				// update layer stack
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
		}

		m_Window->OnUpdate();
	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	//TODO: Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	return false;
}
