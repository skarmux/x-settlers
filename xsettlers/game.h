#pragma once

#include <engine.h>

class Game : public Layer
{
public:
	Game();
	virtual ~Game() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(SDL_Event& e) override;

private:
	// OrthographicCameraController m_CameraController;

	std::shared_ptr<Texture2D> m_GrassTexture;
};