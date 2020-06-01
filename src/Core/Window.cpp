#include "Window.h"

#include "../Platform/Desktop/DesktopWindow.h"
#include "../Platform/Android/AndroidWindow.h"

#include <memory>

Window::PLATFORM Window::s_Platform = Window::PLATFORM::Windows;

std::unique_ptr<Window> Window::Create(const WindowProps& props)
{
	switch (s_Platform) {
	case Window::PLATFORM::Windows: return std::make_unique<DesktopWindow>(props);
	case Window::PLATFORM::Android: return std::make_unique<AndroidWindow>(props);
	}

	return nullptr;
}
