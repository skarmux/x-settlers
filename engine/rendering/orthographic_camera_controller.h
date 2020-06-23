#pragma once

#include "rendering/orthographic_camera.h"
#include "core/timestep.h"

#include <glm/glm.hpp>
#include <SDL.h>

class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio);

	void OnUpdate(Timestep ts);
	void OnEvent(SDL_Event& e);

	OrthographicCamera& GetCamera() { return m_Camera; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }

	float GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(float level) { m_ZoomLevel = level; }

private:
	bool OnMouseScrolled(SDL_MouseWheelEvent& e);
	bool OnWindowResized(SDL_WindowEvent& e);

private:
	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;
	OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
};