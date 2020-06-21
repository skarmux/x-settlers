#include "game.h"

#include "core/layer.h"

#include <glm/glm.hpp>

Game::Game()
	: Layer()
{}

void Game::OnAttach()
{
	//m_GrassTexture = Texture2D::Create("assets/textures/s3_grass.png");

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
}

void Game::OnDetach()
{
	// de-initialize
}

void Game::OnUpdate(Timestep ts)
{
	//m_CameraController.OnUpdate(ts);

	RenderCommand::Clear();

	glm::vec2 position(0,0);
	glm::vec2 size(1,1);
	glm::vec4 color(1,1,1,1);

	Renderer2D::DrawQuad(position, size, color);
	//Renderer2D::DrawQuad(position, size, m_GrassTexture);
}

void Game::OnEvent(SDL_Event& e)
{
	// TODO camera movement on mouse event
	// m_CameraController.OnEvent(e);
}