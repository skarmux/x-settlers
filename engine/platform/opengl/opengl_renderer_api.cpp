#include "platform/opengl/opengl_renderer_api.h"

#include "core/log.h"
#include "platform/opengl/opengl_shader.h"

#include <SDL.h>
#include <glad/glad.h>

static int shaderProgram;
static unsigned int VBO, VAO, EBO;

void OpenGLRendererAPI::Init()
{
	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		CORE_ERROR("Failed to initialize glad!");
		exit(2);
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT (not required for 2D)
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
{
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
