#pragma once

#include "events/event.h"

struct WindowProps
{
	std::string title;
	unsigned int width;
	unsigned int height;

	WindowProps(const std::string& title = "Unknown",
		unsigned int width = 640,
		unsigned int height = 480 )
		:title(title), width(width), height(height) {}
};

class Window
{
public:
	enum class Platform
	{
		Windows, Linux, MacOS, iOS, Android
	};
public:
	// declaring 'event_callback_func' as alias
	using EventCallbackFunc = std::function<void(Event&)>;

	virtual ~Window() {}

	virtual void on_update() = 0;

	virtual unsigned int get_width() const = 0;
	virtual unsigned int get_height() const = 0;

	virtual float get_time() const = 0;

	// Window attributes
	virtual void set_event_callback(const EventCallbackFunc& callback) = 0;
	virtual void set_vsync(bool enabled) = 0;
	virtual bool is_vsync_enabled() const = 0;

	static std::unique_ptr<Window> create(const WindowProps& props = WindowProps());
private:
	static Platform s_platform;
};