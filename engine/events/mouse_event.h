#pragma once

#include "events/event.h"

enum class MouseButton {
	Left, Middle, Right
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(unsigned int y_offset)
		: m_y_offset(y_offset)
	{}

	int get_y_offset() const { return m_y_offset; }

	static EventType get_static_type() {
		return EventType::MouseScrolled;
	}

	EventType get_event_type() const {
		return EventType::MouseScrolled;
	}

	const char* get_name() const {
		return "MouseScrolledEvent";
	}

	int get_category_flags() const {
		return EventCategory::EventCategoryMouse;
	}

private:
	int m_y_offset;
};

class MousePressedEvent : public Event
{
public:

	MousePressedEvent(MouseButton button)
		: m_button(button)
	{}

	MouseButton get_button() const { return m_button; }

	static EventType get_static_type() {
		return EventType::MouseButtonPressed;
	}

	EventType get_event_type() const {
		return EventType::MouseButtonPressed;
	}

	const char* get_name() const {
		return "MousePressedEvent";
	}

	int get_category_flags() const {
		return EventCategory::EventCategoryMouse;
	}

private:
	MouseButton m_button;
};

class MouseReleasedEvent : public Event
{
public:

	MouseReleasedEvent(MouseButton button)
		: m_button(button)
	{}

	MouseButton get_button() const { return m_button; }

	static EventType get_static_type() {
		return EventType::MouseButtonReleased;
	}

	EventType get_event_type() const {
		return EventType::MouseButtonReleased;
	}

	const char* get_name() const {
		return "MouseReleasedEvent";
	}

	int get_category_flags() const {
		return EventCategory::EventCategoryMouse;
	}

private:
	MouseButton m_button;
};

class MouseMovedEvent : public Event
{
public:

	MouseMovedEvent(unsigned int x, unsigned int y, int xRelative, int yRelative)
		: m_x(x), m_y(y), m_x_relative(xRelative), m_y_relative(yRelative)
	{}

	unsigned int GetX() const { return m_x; }
	unsigned int GetY() const { return m_y; }

	int get_x_relative() const { return m_x_relative; }
	int get_y_relative() const { return m_y_relative; }

	static EventType get_static_type() {
		return EventType::MouseMoved;
	}

	EventType get_event_type() const {
		return EventType::MouseMoved;
	}

	const char* get_name() const {
		return "MouseMovedEvent";
	}

	int get_category_flags() const {
		return EventCategory::EventCategoryMouse;
	}

private:
	unsigned int m_x, m_y;
	int m_x_relative, m_y_relative;
};