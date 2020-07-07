#pragma once

#include <EGL/egl.h>

#include "platform/window.h"

class AndroidWindow : public Window
{
public:
	AndroidWindow(const WindowProps& props);
	
	virtual ~AndroidWindow();

	void on_update() override;

	inline unsigned int get_width() const override { return m_data.width; }
	inline unsigned int get_height() const override { return m_data.height; }

	// window attributes
	inline void set_event_callback(const event_callback_func& callback) override { m_data.event_callback = callback; }
	void set_vsync(bool enabled) override;
	bool is_vsync_enabled() const override;

private:
	virtual void init(const WindowProps& props);
	virtual void shutdown();
	
private:
	EGLDisplay* m_Display;
    EGLSurface* m_Surface;
    EGLContext* m_context;

	//std::unique_ptr<GraphicsContext> m_Context;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		bool vsync_enabled;

		event_callback_func event_callback;
	};

	WindowData m_data;
};