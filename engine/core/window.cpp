#include "core/window.h"

#ifdef ANDROID
	#include "platform/android/android_window.h"
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::Android;
#elif WIN32
	#include "platform/sdl_window.h"
	Window::Platform Window::s_platform = Window::Platform::Windows;
#elif UNIX
	#include "platform/sdl_window.h"
	Window::PLATFORM Window::s_Platform = Window::PLATFORM::Linux;
#endif

std::unique_ptr<Window> Window::create(const WindowProps& props)
{
	switch (s_platform)
	{
	case Window::Platform::Windows:
		return std::make_unique<SDLWindow>(props);

	case Window::Platform::Linux:
		return std::make_unique<SDLWindow>(props);

	case Window::Platform::MacOS:
		CORE_ERROR("Platform MacOS is not yet supported!");
		return nullptr;

	case Window::Platform::iOS:
		CORE_ERROR("Platform iOS is not yet supported!");
		return nullptr;

	case Window::Platform::Android:
		CORE_ERROR("Platform Android is not yet supported!");
		return nullptr;

	default:
		CORE_ERROR("No platform selected.");
		return nullptr;
	}
}
