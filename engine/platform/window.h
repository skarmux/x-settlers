#pragma once

#include <functional>
#include <string>
#include <memory>

class Event;

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "X-Settlers",
		unsigned int width = 1280,
		unsigned int height = 720)
		:Title(title), Width(width), Height(height) {}
};

class Window
{
public:
	enum class PLATFORM { SDL, Android };

public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

private:
	static PLATFORM s_Platform;
};