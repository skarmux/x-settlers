#include "layer/landscape.h"

#include <glm/glm.hpp>

Landscape::Landscape() : Layer(), m_CameraController(1280.0f/720.0f) { }

void Landscape::OnAttach()
{
	// load required textures
	m_GrassTexture = Texture2D::Create("assets/textures/grass.png");
	m_GrassTexture->Bind();

	// load Map
}

void Landscape::OnDetach()
{

}

void Landscape::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	
	RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	RenderCommand::Clear();

	glm::vec2 position(0.0f, 0.0f);

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad(position, m_GrassTexture);
	Renderer2D::EndScene();
}

void Landscape::OnEvent(SDL_Event& e)
{

}