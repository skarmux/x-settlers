#include "X-Settlers.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>

const std::string MAP_PATH = "maps/4Circles.map";
const std::string GFX_LANDSCAPE_PATH = "gfx/Siedler3_00.f8007e01f.dat";
const std::string GFX_OBJECTS_PATH = "gfx/Siedler3_01.f8007e01f.dat";
const std::string GFX_BUILDINGS_PATH = "gfx/Siedler3_13.f8007e01f.dat";

XSettlers::XSettlers() {
	m_Window = std::shared_ptr<Window>(Window::Create());
}

void XSettlers::Run() {

	Renderer r;
	r.Init(m_Window);

	PlayField pf(MAP_PATH);

	m_Running = true;
	while (m_Running) {
		//glClearColor(1,0,1,1);
		//glClear(GL_COLOR_BUFFER_BIT);
		pf.Update();
		r.Draw(m_Vertices);
		m_Window->OnUpdate();
	}
}

int main(int argc, char** argv) {

	Log::Init();
	CORE_INFO("Initialized Log!");

	XSettlers* app = new XSettlers();

	app->Run();

	return 0;
}