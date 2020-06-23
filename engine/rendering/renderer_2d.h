#pragma once

#include "rendering/renderer_api.h"
#include "rendering/texture.h"
#include "rendering/orthographic_camera.h"

#include <glm/glm.hpp>
#include <SDL.h>

class Renderer2D
{
public:
	static void Init();
	static void Shutdown();

	static void OnWindowResize(uint32_t width, uint32_t height);

	static void BeginScene(const OrthographicCamera& camera);
	static void EndScene();
	static void Flush();

	static void DrawTriangle(const glm::vec2& position, const std::shared_ptr<Texture2D>& texture);
	static void DrawQuad(const glm::vec2& position, const std::shared_ptr<Texture2D>& texture);

	//static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
	static void FlushAndReset();
};
