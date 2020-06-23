#pragma once

#include "rendering/vertex.h"
#include "rendering/vertex_array.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class RendererAPI
{
public:
	enum class API { OpenGL = 0, Vulkan = 1 };
public:
	virtual void Init() = 0;
	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

	static API GetAPI() { return s_API; }
	static std::unique_ptr<RendererAPI> Create();

private:
	static API s_API;

};
