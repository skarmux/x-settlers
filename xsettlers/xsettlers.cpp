#include <engine.h>
#include <core/entry_point.h>

#include "game.h"

class XSettlers : public Application
{
public:
	XSettlers()
	{
		PushLayer(new Game());
	}

	~XSettlers(){}
};

Application* CreateApplication()
{
	return new XSettlers();
}
