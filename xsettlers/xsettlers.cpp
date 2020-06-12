#include <engine.h>

int main(int argc, char** argv)
{
    Log::Init();

	auto win = Window::Create();

	Renderer2D::Init();

	// Shader shader = Shader::create("Texture Shader", "/texture.glsl");

	// enter GAME LOOP
	while (true)
	{
		RenderCommand::Clear();

		// TODO: draw rectangle
		glm::vec2 position(0,0);
		glm::vec2 size(1,1);
		glm::vec4 color(1,1,1,1);
		Renderer2D::DrawQuad(position, size, color);

		win->OnUpdate();
	}

	Renderer2D::Shutdown();

	return 0;
}
