#pragma once

#include <engine.h>

class Interface : public Layer
{
public:
	Interface();
	virtual ~Interface() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(SDL_Event& e) override;
};