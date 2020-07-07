#pragma once

enum class EventType
{
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	EventCategoryApplication,
	EventCategoryInput,
	EventCategoryKeyboard,
	EventCategoryMouse,
	EventCategoryMouseButton
};

class Event
{
public:
	bool m_handled = false;

	virtual EventType get_event_type() const = 0;
	virtual const char* get_name() const = 0;
	virtual int get_category_flags() const = 0;

	bool is_in_category(EventCategory category)
	{
		return get_category_flags() & category;
	}
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_event(event)
	{}

	// F will be deduced by the compiler
	template<typename T, typename F>
	bool dispatch(const F& func)
	{
		if (m_event.get_event_type() == T::get_static_type())
		{
			m_event.m_handled = func(static_cast<T&>(m_event));
			return true;
		}
		
		return false;
	}

private:
	Event& m_event;
};
