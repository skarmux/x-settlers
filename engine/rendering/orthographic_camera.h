#pragma once

#include <glm/glm.hpp>

class OrthographicCamera
{
public:
	OrthographicCamera(float left, float right, float bottom, float top);

	void set_projection(float left, float right, float bottom, float top);

	const glm::vec3& get_position() const { return m_position; }
	void set_position(const glm::vec3& position) { m_position = position; recalculate_view_matrix(); }

	const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
	const glm::mat4& get_view_matrix() const { return m_view_matrix; }
	const glm::mat4& get_view_projection_matrix() const { return m_view_projection_matrix; }
private:
	void recalculate_view_matrix();
private:
	glm::mat4 m_projection_matrix;
	glm::mat4 m_view_matrix;
	glm::mat4 m_view_projection_matrix;

	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	float m_rotation = 0.0f;
};