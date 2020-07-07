#include <engine.h>
#include <core/entry_point.h>

#include "layer/landscape.h"

class XSettlers : public Application
{
public:
	XSettlers()
	{
		push_layer(new Landscape());
		//PushLayer(new Game());
		//PushLayer(new Interface());
	}

	~XSettlers() {}
};

Application* create_application()
{
	return new XSettlers();
}
