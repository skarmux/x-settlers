#include "renderer_2d.h"
#include "render_command.h"

void Renderer2D::Init()
{
	RenderCommand::Init();
}

void Renderer2D::Shutdown()
{

}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{

}

void Renderer2D::EndScene()
{
	Flush();
}

void Renderer2D::Flush()
{
	//RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
}

void Renderer2D::FlushAndReset()
{
	EndScene();

	// ...
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	//DrawQuad({ position.x, position.y, 0.0f }, size, color);

	std::vector<Vertex> vertexBuffer;

	Vertex v0{};
	v0.pos = position;

	Vertex v1{};
	v1.pos = position + glm::vec2(0,1);

	Vertex v2{};
	v2.pos = position + glm::vec2(1, 0);

	Vertex v3{};
	v3.pos = position + glm::vec2(1, 1);

	vertexBuffer.push_back(v0);
	vertexBuffer.push_back(v1);
	vertexBuffer.push_back(v2);
	vertexBuffer.push_back(v3);

	RenderCommand::DrawIndexed();
}

//void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
//{
//
//}

void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}


