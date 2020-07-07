#pragma once

#include "core/time_delta.h"
#include "events/event.h"

class Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void on_attach() {};
	virtual void on_detach() {};

	virtual void on_update(TimeDelta ts) {};
	virtual void on_event(Event& event) {};
};
