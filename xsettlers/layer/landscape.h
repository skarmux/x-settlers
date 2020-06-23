#pragma once

#include <engine.h>

class Landscape : public Layer
{
public:
	Landscape();
	virtual ~Landscape() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(SDL_Event& e) override;

private:
	OrthographicCameraController m_CameraController;

	std::shared_ptr<Texture2D> m_GrassTexture;
};