#include "platform/sdl_window.h"

#include "rendering/renderer_2d.h"

#include "events/event.h"
#include "events/application_event.h"
#include "events/mouse_event.h"

SDLWindow::SDLWindow(const WindowProps& props)
{
	init(props);
}

SDLWindow::~SDLWindow()
{
	shutdown();
}

void SDLWindow::init(const WindowProps& props)
{
	m_data.title = props.title;
	m_data.width = props.width;
	m_data.height = props.height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CORE_ERROR("Could not initialize SDL! SDL Error: %s", SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);

	// create the window
	if (Renderer2D::get_api() == RendererAPI::API::OpenGL) {
		m_window = SDL_CreateWindow(m_data.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_data.width, (int)m_data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ); // SDL_WINDOW_BORDERLESS is fancy, but removes resize, mini-/maximise and close functionality
	}
	else if (Renderer2D::get_api() == RendererAPI::API::Vulkan) {
		//m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_Data.Width, (int)m_Data.Height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
	}

	// TODO: On Apple's OS X you must set the NSHighResolutionCapable Info.plist property to YES, otherwise you will not receive a High DPI OpenGL canvas.

	if (m_window == nullptr) {
		CORE_ERROR("Couldn't set video mode!");
		exit(2);
	}
	// trap mouse cursor in window and only generate relative mouse motion events
	// SDL_SetRelativeMouseMode(SDL_TRUE);

	/*SDL_Cursor* cursor;
	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(cursor);*/

	m_arrow_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_sizeall_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

	m_context = GraphicsContext::create(m_window);
	m_context->init();

	set_vsync(true); // default setting
}

void SDLWindow::shutdown() 
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void SDLWindow::on_update()
{	
	while (SDL_PollEvent(&m_event) != 0)
	{
		switch (m_event.type)
		{
		case SDL_WINDOWEVENT:
			switch (m_event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				EVENT_INFO("Window Resize Event");
				m_data.event_callback(WindowResizeEvent(m_event.window.data1, m_event.window.data2));
				break;
			case SDL_WINDOWEVENT_CLOSE:
				EVENT_INFO("Window Close Event");
				m_data.event_callback(WindowCloseEvent());
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			m_data.event_callback(MouseScrolledEvent(m_event.wheel.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (m_event.button.button) {
			case SDL_BUTTON_LEFT:
				break;
			case SDL_BUTTON_RIGHT:
				EVENT_INFO("mouse button right pressed");
				SDL_SetCursor(m_sizeall_cursor);
				m_data.event_callback(MousePressedEvent(MouseButton::Right));
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (m_event.button.button) {
			case SDL_BUTTON_LEFT:
				break;
			case SDL_BUTTON_RIGHT:
				EVENT_INFO("mouse button right released");
				SDL_SetCursor(m_arrow_cursor);
				m_data.event_callback(MouseReleasedEvent(MouseButton::Right));
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			//EVENT_INFO("mouse moved {0}xRel {1}yRel {2}x {3}y", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);
			m_data.event_callback(MouseMovedEvent( m_event.motion.x, m_event.motion.y, m_event.motion.xrel, m_event.motion.yrel ));
			break;
		default:
			EVENT_WARN("event not handled ({0})", m_event.type);
		}
	}
	m_context->swap_buffers();
}

void SDLWindow::set_vsync(bool enabled) {
	if (enabled) {
		m_context->set_swap_interval(1);
	}	
	else {
		m_context->set_swap_interval(0);
	}

	m_data.vsync_enabled = enabled;
}

bool SDLWindow::is_vsync_enabled() const {
	return m_data.vsync_enabled;
}
