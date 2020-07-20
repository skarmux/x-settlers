#include "rendering/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	: m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_view_matrix(1.0f)
{
	m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

void OrthographicCamera::set_projection(float left, float right, float bottom, float top)
{
	m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

void OrthographicCamera::recalculate_view_matrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);

	m_view_matrix = glm::inverse(transform);
	m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}