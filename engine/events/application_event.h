#pragma once

#include "events/event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_width(width), m_height(height) {}

	unsigned int get_width() const { return m_width; }
	unsigned int get_height() const { return m_height; }

	static EventType get_static_type() {
		return EventType::WindowResize;
	}

	EventType get_event_type() const {
		return EventType::WindowResize;
	}
	
	const char* get_name() const {
		return "WindowResizeEvent";
	}
	
	int get_category_flags() const {
		return EventCategory::EventCategoryApplication;
	}

private:
	unsigned int m_width, m_height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	static EventType get_static_type() {
		return EventType::WindowClose;
	}

	EventType get_event_type() const {
		return EventType::WindowClose;
	}

	const char* get_name() const {
		return "WindowCloseEvent";
	}

	int get_category_flags() const {
		return EventCategory::EventCategoryApplication;
	}
};
