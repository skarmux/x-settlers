#pragma once

#include "core/timestep.h"

#include <SDL.h>

class Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void OnAttach() {};
	virtual void OnDetach() {};

	virtual void OnUpdate(Timestep ts) {};
	virtual void OnEvent(SDL_Event& event) {};
};
