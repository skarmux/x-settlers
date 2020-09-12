#pragma once

#include "core/window.h"

#include <GLFW/glfw3.h>

class GNUWindow : public Window
{
public:
	GNUWindow(const WindowProps& props);
	~GNUWindow();

	void on_update() override;

	uint32_t get_width() const override { return m_width; }
	uint32_t get_height() const override { return m_height; }

	float get_time() const override { return glfwGetTime(); };

	void set_event_callback(const EventCallbackFunc& callback) override { m_event_callback = callback; }
	void set_vsync(bool enabled) override;
	bool is_vsync_enabled() const override { return m_vsync_enabled; }
private:
	GLFWwindow* m_window;

	std::string m_title;
	uint32_t m_width, m_height;
	bool m_vsync_enabled;

	double m_mouse_x, m_mouse_y;

	EventCallbackFunc m_event_callback;
};