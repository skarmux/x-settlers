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
	OrthographicCameraController(float aspectRatio);
	OrthographicCameraController(float width, float height);

	void on_update(TimeDelta ts);
	void on_event(Event& e);

	OrthographicCamera& get_camera() { return m_camera; }
	const OrthographicCamera& get_camera() const { return m_camera; }

	float get_zoom_level() const { return m_zoom_level; }
	void set_zoom_level(float level) {
		m_zoom_level = level;
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
	}

	void set_position(const glm::vec3& position) { m_camera_position = position; };
private:
	bool on_mouse_pressed(MousePressedEvent& e);
	bool on_mouse_released(MouseReleasedEvent& e);
	bool on_mouse_moved(MouseMovedEvent& e);
	bool on_mouse_scrolled(MouseScrolledEvent& e);
	bool on_window_resized(WindowResizeEvent& e);
private:
	float m_aspect_ratio;
	float m_zoom_level;

	bool m_move_key_pressed;

	OrthographicCamera m_camera;

	glm::vec3 m_camera_position;
};