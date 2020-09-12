#pragma once

#include "core/window.h"

#include <SDL.h>

class SDLWindow : public Window
{
public:
	SDLWindow(const WindowProps& props);
	~SDLWindow();

	void on_update() override;

	uint32_t get_width() const override { return m_width; }
	uint32_t get_height() const override { return m_height; }

	float get_time() const override { return SDL_GetTicks(); };

	void set_event_callback(const EventCallbackFunc& callback) override { m_event_callback = callback; }
	void set_vsync(bool enabled) override;
	bool is_vsync_enabled() const override { return m_vsync_enabled; }
private:
	SDL_Window* m_window;
	SDL_GLContext m_context;

	std::string m_title;
	uint32_t m_width, m_height;
	bool m_vsync_enabled;

	EventCallbackFunc m_event_callback;

	SDL_Cursor* m_arrow_cursor;
	SDL_Cursor* m_sizeall_cursor;
};