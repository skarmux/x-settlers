#pragma once

#include "renderer_api.h"

#include <glm/glm.hpp>

class OrthographicCamera;

class Renderer2D
{
public:
	static void Init();
	static void Shutdown();

	static void OnWindowResize(uint32_t width, uint32_t height);

	static void BeginScene(const OrthographicCamera& camera);
	static void EndScene();
	static void Flush();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

	//static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
	static void FlushAndReset();
};
