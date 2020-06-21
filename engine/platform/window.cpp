#include "window.h"

#include <memory>

#ifdef ANDROID
	#include "platform/android/android_window.h"
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::Android;
#else
	#include "platform/sdl_window.h"
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::Desktop;
#endif

std::unique_ptr<Window> Window::Create(const WindowProps& props)
{
	switch (s_Platform) {
	case Window::PLATFORM::Desktop:
		return std::make_unique<SDLWindow>(props);
	case Window::PLATFORM::Android:
		return nullptr;//return std::make_unique<AndroidWindow>(props);
	}

	return nullptr;
}
