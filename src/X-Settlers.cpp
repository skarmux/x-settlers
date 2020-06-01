#include <SDL.h>
#include "Core/Log.h"
#include "Core/Window.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderCommand.h"

// There needs to be a X-Settlers class later on to manage closing events, game states, menues, etc...
class XSettlers;

int main(int argc, char** argv) {

	Log::Init();
	CORE_INFO("Initialized Log!");

	auto win = Window::Create();
	CORE_INFO("Created Window!");

	// TODO: Set Window Event Callback

	Renderer2D::Init();
	CORE_INFO("Initialized Renderer!");

	bool running = true;
	SDL_Event event;

	// enter GAME LOOP
	while (running)
	{
		RenderCommand::Clear();

		// TODO: draw rectangle
		glm::vec2 position(0,0);
		glm::vec2 size(1,1);
		glm::vec4 color(1,1,1,1);
		Renderer2D::DrawQuad(position, size, color);

		win->OnUpdate();
		// handle SDL event queue
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	Renderer2D::Shutdown();

	return EXIT_SUCCESS;
}
