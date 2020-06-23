#include <engine.h>
#include <core/entry_point.h>

#include "layer/landscape.h"
#include "layer/game.h"

class XSettlers : public Application
{
public:
	XSettlers()
	{
		PushLayer(new Landscape());
		PushLayer(new Game());
		//PushLayer(new Interface());
	}

	~XSettlers() {}
};

Application* CreateApplication()
{
	return new XSettlers();
}
