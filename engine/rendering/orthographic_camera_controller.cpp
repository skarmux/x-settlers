#include "rendering/orthographic_camera_controller.h"

void OrthographicCameraController::on_update(TimeDelta ts)
{
	m_camera.set_camera_position(m_camera_position);
}

void OrthographicCameraController::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::on_mouse_scrolled, this, std::placeholders::_1));
	dispatcher.dispatch<MouseMovedEvent>(std::bind(&OrthographicCameraController::on_mouse_moved, this, std::placeholders::_1));
	dispatcher.dispatch<MousePressedEvent>(std::bind(&OrthographicCameraController::on_mouse_pressed, this, std::placeholders::_1));
	dispatcher.dispatch<MouseReleasedEvent>(std::bind(&OrthographicCameraController::on_mouse_released, this, std::placeholders::_1));
	dispatcher.dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::on_window_resized, this, std::placeholders::_1));
}

bool OrthographicCameraController::on_mouse_moved(MouseMovedEvent& e)
{
	if (m_move_key_pressed)
	{
		m_camera_position.x += e.get_x_relative() * m_zoom_level;
		m_camera_position.y -= e.get_y_relative() * m_zoom_level;
	}
	
	return true;
}

bool OrthographicCameraController::on_mouse_pressed(MousePressedEvent& e)
{
	if (e.get_button() == MouseButton::Right)
		m_move_key_pressed = true;

	return false;
}

bool OrthographicCameraController::on_mouse_released(MouseReleasedEvent& e)
{
	if (e.get_button() == MouseButton::Right)
		m_move_key_pressed = false;

	return false;
}

bool OrthographicCameraController::on_mouse_scrolled(MouseScrolledEvent& e)
{	
	constexpr float MIN_ZOOM_LEVEL = 0.25f;
	constexpr float MAX_ZOOM_LEVEL = 4.0f;

	// fix zoom level de-/increase for integer scaling
	// and to ignore OS scrolling settings
	if (e.get_y_offset() > 0)
		m_zoom_level *= 0.5f;
	else
		m_zoom_level *= 2.0f;
	
	m_zoom_level = std::max(m_zoom_level, MIN_ZOOM_LEVEL);
	//m_zoom_level = std::min(m_zoom_level, MAX_ZOOM_LEVEL);
	
	m_camera.set_projection(
		-((float)m_width / 2.0f) * m_zoom_level,   // left
		((float)m_width / 2.0f) * m_zoom_level,    // right
		-((float)m_height / 2.0f) * m_zoom_level,  // top
		((float)m_height / 2.0f) * m_zoom_level ); // bottom

	return false;
}

bool OrthographicCameraController::on_window_resized(WindowResizeEvent& e)
{
	m_width = e.get_width();
	m_height = e.get_height();

	m_camera.set_projection(
		-((float)m_width / 2.0f) * m_zoom_level,   // left
		((float)m_width / 2.0f) * m_zoom_level,    // right
		-((float)m_height / 2.0f) * m_zoom_level,  // top
		((float)m_height / 2.0f) * m_zoom_level); // bottom

	return false;
}