#pragma once

#include "../../Core/Window.h"
#include "../../Core/Log.h"

#include <SDL.h>

class AndroidWindow : public Window
{
public:
	AndroidWindow(const WindowProps& props);
	virtual ~AndroidWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// window attributes
	inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

private:
	virtual void Initialize(const WindowProps& props);
	virtual void Shutdown();
private:
	SDL_Window* m_Window;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};