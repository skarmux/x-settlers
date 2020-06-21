#pragma once

#include "events/event.h"
#include "core/input.h"

class KeyEvent
{
public: 
	KeyCode GetKeyCode() const { return m_KeyCode; }

	//EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(KeyCode keycode)
		: m_KeyCode(keycode) {}

	KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyCode keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	int GetRepeatCount() const { return m_RepeatCount; }

	//EVENT_CLASS_TYPE(KeyPressed)
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(KeyCode keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	int GetRepeatCount() const { return m_RepeatCount; }

	//EVENT_CLASS_TYPE(KeyPressed)
private:
	int m_RepeatCount;
};
