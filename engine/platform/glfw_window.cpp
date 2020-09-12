#include "platform/glfw_window.h"

#include "rendering/renderer_2d.h"

#include "events/event.h"
#include "events/application_event.h"
#include "events/mouse_event.h"

static void GLFWErrorCallback(int error, const char* description)
{
	CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

GNUWindow::GNUWindow(const WindowProps& props)
	: m_title(props.title), m_width(props.width), m_height(props.height)
{
	int success = glfwInit();

	// create the window
	switch (Renderer2D::get_api()) {
	case RendererAPI::API::OpenGL:
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	}

	// TODO: On Apple's OS X you must set the NSHighResolutionCapable Info.plist property to YES, otherwise you will not receive a High DPI OpenGL canvas.

	if (m_window == nullptr) {
		CORE_ERROR("Couldn't set video mode!");
		exit(2);
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);
	set_vsync(false);

	glfwSetErrorCallback(GLFWErrorCallback);

	glfwSetWindowSizeCallback(m_window,
		[](GLFWwindow* window, int width, int height)
		{
			GNUWindow* my_window = (GNUWindow*)glfwGetWindowUserPointer(window);
			my_window->m_width = width;
			my_window->m_height = height;

			WindowResizeEvent event(width, height);
			my_window->m_event_callback(event);
		} );

	glfwSetWindowCloseCallback(m_window,
		[](GLFWwindow* window)
		{
			GNUWindow* my_window = (GNUWindow*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			my_window->m_event_callback(event);
		} );

	glfwSetScrollCallback(m_window,
		[](GLFWwindow* window, double xOffset, double yOffset)
		{
			GNUWindow* my_window = (GNUWindow*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((unsigned int)yOffset);
			my_window->m_event_callback(event);
		} );

	glfwSetMouseButtonCallback(m_window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			GNUWindow* my_window = (GNUWindow*)glfwGetWindowUserPointer(window);

			MouseButton mouse_button = static_cast<MouseButton>(button);

			switch (action) {
			case GLFW_PRESS:
				my_window->m_event_callback(MousePressedEvent(mouse_button));
				break;
			case GLFW_RELEASE:
				my_window->m_event_callback(MouseReleasedEvent(mouse_button));
				break;
			}
		} );

	glfwSetCursorPosCallback(m_window,
		[](GLFWwindow* window, double xPos, double yPos)
		{
			GNUWindow* my_window = (GNUWindow*)glfwGetWindowUserPointer(window);

			int32_t offset_x = my_window->m_mouse_x - xPos;
			int32_t offset_y = my_window->m_mouse_y - yPos;

			my_window->m_mouse_x = xPos;
			my_window->m_mouse_y = yPos;

			//EVENT_INFO("mouse moved {0}xRel {1}yRel {2}x {3}y", offset_x, offset_y, xPos, yPos);

			MouseMovedEvent event((unsigned int)xPos, (unsigned int)yPos, offset_x, offset_y);
			my_window->m_event_callback(event);
		} );
}

GNUWindow::~GNUWindow()
{
	glfwDestroyWindow(m_window);
}

void GNUWindow::on_update()
{	
	glfwPollEvents();
	
	glfwSwapBuffers(m_window);
}

void GNUWindow::set_vsync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_vsync_enabled = enabled;
}
