#include "rendering/orthographic_camera_controller.h"

OrthographicCameraController::OrthographicCameraController(float aspect_ratio)
	: m_aspect_ratio(aspect_ratio),
	m_zoom_level(1.0f),
	m_move_key_pressed(false),
	m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level),
	 m_camera_position(glm::vec3(0.0f))
{}

OrthographicCameraController::OrthographicCameraController(float width, float height)
	: m_aspect_ratio(width / height),
	m_zoom_level(height / 2.0f),
	m_move_key_pressed(false),
	m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio* m_zoom_level, -m_zoom_level, m_zoom_level),
	m_camera_position(glm::vec3(0.0f))
{}

void OrthographicCameraController::on_update(TimeDelta ts)
{
	m_camera.set_position(m_camera_position);
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

bool OrthographicCameraController::on_mouse_moved(MouseMovedEvent& e) {
	// TODO
	// camera.position + ((screenspace position - screen middle) / screen height/2 * ortho size
	// e.g. we want the screen position (400,400) with the camera at 0,0,0 with a screen resolution of 800,600 and ortho size 10:
	// (0,0,0) + (( (400,400) - (400,300) ) / (800/2) * 10) = (0,0,0) + ((0,100) / 400 * 10) = (0,0,0) + (0,2.5).
	// So the screen coordinate of (400,400) maps to (0,2.5) with those camera settings.
	
	//float mouse_world_pos = ((float)e.GetX() - (1280.0 / 2.0)) / (1280.0 / 2.0) * m_ZoomLevel;
	
	//RENDERER_INFO("Camera offset on x axis : {0}", 1.0+mouse_world_offset);

	// (    left,       right,    bottom, top)
	// (-1.77 * zoom, 1.0 * zoom, -zoom, +zoom)

	if (m_move_key_pressed)
	{
		m_camera_position.x -= e.get_x_relative();
		m_camera_position.y += e.get_y_relative();
	}
	
	return false;
}

bool OrthographicCameraController::on_mouse_pressed(MousePressedEvent& e) {
	if (e.get_button() == MouseButton::Right)
		m_move_key_pressed = true;
	return false;
}

bool OrthographicCameraController::on_mouse_released(MouseReleasedEvent& e) {
	if (e.get_button() == MouseButton::Right)
		m_move_key_pressed = false;
	return false;
}

bool OrthographicCameraController::on_mouse_scrolled(MouseScrolledEvent& e)
{
	// TODO: make zoom level relative to aspect ratio, so that it stays on the same zoom on window resize
	
	constexpr float MIN_ZOOM_LEVEL = 1.0f;

	m_zoom_level -= e.get_y_offset() * 50;
	m_zoom_level = std::max(m_zoom_level, MIN_ZOOM_LEVEL);
	
	m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
	return false;
}

bool OrthographicCameraController::on_window_resized(WindowResizeEvent& e)
{
	m_aspect_ratio = (float)e.get_width() / (float)e.get_height();
	//m_zoom_level = (float)e.get_height() / 2.0f;
	m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
	return false;
}