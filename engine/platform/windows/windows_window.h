#pragma once

#include "core/window.h"

#include <Windows.h>

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProps& props);
	~WindowsWindow();

	void on_update() override;

	uint32_t get_width() const override { return m_width; }
	uint32_t get_height() const override { return m_height; }

	float get_time() const override;

	void set_event_callback(const event_callback_func& callback) override { m_event_callback = callback; }
	void set_vsync(bool enabled) override;
	bool is_vsync_enabled() const override { return m_vsync_enabled; }
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	std::string m_title;
	uint32_t m_width, m_height;
	bool m_vsync_enabled;

	event_callback_func m_event_callback;

	HWND m_window;
};