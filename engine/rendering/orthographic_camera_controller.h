#pragma once

#include <glm/glm.hpp>

#include "rendering/orthographic_camera.h"

#include "core/time_delta.h"

#include "events/event.h"
#include "events/application_event.h"
#include "events/mouse_event.h"

class OrthographicCameraController
{
public:
	OrthographicCameraController(uint32_t width, uint32_t height) : 
		m_width(width),	m_height(height),
		m_zoom_level(1.0f),
		m_move_key_pressed(false),
		m_camera(-(width / 2.0f), (width / 2.0f), -(height / 2.0f), (height / 2.0f)),
		m_camera_position(glm::vec3(0.0f) )
	{}

	void on_update(TimeDelta ts);
	void on_event(Event& e);

	OrthographicCamera& get_camera() { return m_camera; }
	const OrthographicCamera& get_camera() const { return m_camera; }

	void set_camera_position(const glm::vec3& position) { m_camera_position = position; };
private:
	bool on_mouse_pressed(MousePressedEvent& e);
	bool on_mouse_released(MouseReleasedEvent& e);
	bool on_mouse_moved(MouseMovedEvent& e);
	bool on_mouse_scrolled(MouseScrolledEvent& e);
	bool on_window_resized(WindowResizeEvent& e);
private:
	float m_zoom_level;
	uint32_t m_width, m_height;

	bool m_move_key_pressed;

	OrthographicCamera m_camera;

	glm::vec3 m_camera_position;
};