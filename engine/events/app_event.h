#pragma once

#include "events/event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height) {}

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
private:
	unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() = default;
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() = default;
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() = default;
};