#include <engine.h>
#include <core/entry_point.h>

#include "layer/terrain_layer.h"

const uint32_t WIN_WIDTH = 1280;
const uint32_t WIN_HEIGHT = 720;

class XSettlers : public Application
{
public:
	XSettlers() : 
		Application("X-Settlers", WIN_WIDTH, WIN_HEIGHT)
	{
		push_layer(new TerrainLayer(WIN_WIDTH, WIN_HEIGHT));
	}

	~XSettlers() {}
};

Application* create_application()
{
	return new XSettlers();
}
