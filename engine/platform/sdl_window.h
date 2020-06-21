#pragma once

#include "platform/window.h"
#include "rendering/graphics_context.h"

#include <SDL.h>

class SDLWindow : public Window
{
public:
	SDLWindow(const WindowProps& props);
	
	virtual ~SDLWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// window attributes
	inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();

private:
	SDL_Window* m_Window;
	std::unique_ptr<GraphicsContext> m_Context;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};
