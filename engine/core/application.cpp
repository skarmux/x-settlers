#include "core/application.h"
#include "core/time_delta.h"

#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"

#include "events/mouse_event.h"

#include <Windows.h>

Application* Application::s_instance = nullptr;

Application::Application(const std::string& name, uint32_t width, uint32_t height)
{
	s_instance = this;

	WindowProps properties{};
	properties.title = name;
	properties.width = width;
	properties.height = height;

	m_window = Window::create(properties);
	m_window->set_event_callback(std::bind(&Application::on_event, this, std::placeholders::_1));

	RenderCommand::init();
	Renderer2D::init();
}

Application::~Application()
{
	Renderer2D::shutdown();
}

void Application::push_layer(Layer* layer)
{
	m_layer_stack.push_layer(layer);
	layer->on_attach();
}

void Application::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::on_window_close, this, std::placeholders::_1));
	dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::on_window_resize, this, std::placeholders::_1));

	for (auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); ++it)
	{
		if (e.m_handled)
			break;
		(*it)->on_event(e);
	}
}

void Application::run()
{	
	while (m_running)
	{
		float time = m_window->get_time();
		TimeDelta time_delta = time - m_last_frame_time;
		m_last_frame_time = time;

		if (!m_minimized)
		{
			{
				// update layer stack
				for (Layer* layer : m_layer_stack)
					layer->on_update(time_delta);
			}
		}

		m_window->on_update();
	}
}

bool Application::on_window_close(WindowCloseEvent& e)
{
	CORE_INFO("received WindowCloseEvent()");
	m_running = false;
	return true;
}

bool Application::on_window_resize(WindowResizeEvent& e)
{
	CORE_INFO("received WindowResizeEvent()");
	if (e.get_width() == 0 || e.get_height() == 0)
	{
		m_minimized = true;
		return false;
	}

	m_minimized = false;
	Renderer2D::on_window_resize(e.get_width(), e.get_height());

	return false;
}
