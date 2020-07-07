#pragma once

#include "core/window.h"

#include "rendering/graphics_context.h"

#include <SDL.h>

class SDLWindow : public Window
{
public:
	SDLWindow(const WindowProps& props);
	~SDLWindow();

	void on_update() override;

	unsigned int get_width() const override { return m_data.width; }
	unsigned int get_height() const override { return m_data.height; }

	void set_event_callback(const event_callback_func& callback) override { m_data.event_callback = callback; }
	void set_vsync(bool enabled) override;
	bool is_vsync_enabled() const override;
private:
	virtual void init(const WindowProps& props);
	virtual void shutdown();
private:
	SDL_Window* m_window;
	std::unique_ptr<GraphicsContext> m_context;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		bool vsync_enabled;

		event_callback_func event_callback;
	};

	WindowData m_data;

	SDL_Event m_event;

	SDL_Cursor* m_arrow_cursor;
	SDL_Cursor* m_sizeall_cursor;
};