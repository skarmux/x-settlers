#include "window.h"

#include "platform/sdl_window.h"
#include "platform/android/android_window.h"

#include <memory>

#ifdef WIN32
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::SDL;
#elif ANDROID
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::Android;
#endif

std::unique_ptr<Window> Window::Create(const WindowProps& props)
{
	switch (s_Platform) {
	case Window::PLATFORM::SDL:
		return std::make_unique<DesktopWindow>(props);
	case Window::PLATFORM::Android:
		return nullptr;//return std::make_unique<AndroidWindow>(props);
	}

	return nullptr;
}
