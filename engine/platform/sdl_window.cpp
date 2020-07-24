#include "platform/sdl_window.h"

#include "rendering/renderer_2d.h"

#include "events/event.h"
#include "events/application_event.h"
#include "events/mouse_event.h"

SDLWindow::SDLWindow(const WindowProps& props)
	: m_title(props.title), m_width(props.width), m_height(props.height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CORE_ERROR("Could not initialize SDL! SDL Error: %s", SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);

	// create the window
	if (Renderer2D::get_api() == RendererAPI::API::OpenGL) {
		m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_width, (int)m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); // SDL_WINDOW_BORDERLESS is fancy, but removes resize, mini-/maximise and close functionality
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
	
	//SDL_ShowCursor(SDL_ENABLE);

	/*SDL_Cursor* cursor;
	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(cursor);*/
	

	m_arrow_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_sizeall_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

	// setup OpenGL graphics context

	//m_context = GraphicsContext::create(m_window);
	//m_context->init();
	m_context = SDL_GL_CreateContext(m_window);
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
	SDL_GL_MakeCurrent(m_window, m_context);

	set_vsync(true); // default setting
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void SDLWindow::on_update()
{	
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT:

			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				EVENT_INFO("Window Resize Event");
				m_event_callback(WindowResizeEvent(event.window.data1, event.window.data2));
				break;

			case SDL_WINDOWEVENT_CLOSE:
				EVENT_INFO("Window Close Event");
				m_event_callback(WindowCloseEvent());
				break;
			}
			break;

		case SDL_KEYDOWN:

			// https://wiki.libsdl.org/SDL_Keycode
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_event_callback(WindowCloseEvent());
				break;
			}
			break;

		case SDL_MOUSEWHEEL:

			m_event_callback(MouseScrolledEvent(event.wheel.y));
			break;

		case SDL_MOUSEBUTTONDOWN:

			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				break;

			case SDL_BUTTON_RIGHT:
				EVENT_INFO("mouse button right pressed");
				SDL_SetCursor(m_sizeall_cursor);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				m_event_callback(MousePressedEvent(MouseButton::Right));
				break;
			}
			break;

		case SDL_MOUSEBUTTONUP:

			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				break;

			case SDL_BUTTON_RIGHT:
				EVENT_INFO("mouse button right released");
				SDL_SetCursor(m_arrow_cursor);
				SDL_SetRelativeMouseMode(SDL_FALSE);
				SDL_WarpMouseInWindow(m_window, m_width / 2, m_height / 2);
				m_event_callback(MouseReleasedEvent(MouseButton::Right));
				break;
			}
			break;

		case SDL_MOUSEMOTION:

			//EVENT_INFO("mouse moved {0}xRel {1}yRel {2}x {3}y", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);
			m_event_callback(MouseMovedEvent( event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel ));
			break;

		default:

			EVENT_WARN("event not handled ({0})", event.type);
		}
	}

	SDL_GL_SwapWindow(m_window);
}

void SDLWindow::set_vsync(bool enabled)
{
	if (enabled) {
		//m_context->set_swap_interval(1);
		SDL_GL_SetSwapInterval(1);
	}	
	else {
		//m_context->set_swap_interval(0);
		SDL_GL_SetSwapInterval(0);
	}

	m_vsync_enabled = enabled;
}
