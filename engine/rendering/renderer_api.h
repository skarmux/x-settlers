#pragma once

#include <glm/glm.hpp>

#include "rendering/vertex_array.h"

class RendererAPI
{
public:
	enum class API 
	{ 
		None, Software, OpenGL, OpenGLES, Vulkan, DirectX, Metal 
	};
public:
	virtual void init() = 0;
	virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void set_clear_color(const glm::vec4& color) = 0;
	virtual void clear() = 0;

	//virtual void draw_indexed(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer, uint32_t count = 0) = 0;
	virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;
	virtual void draw(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0) = 0;

	static API get_api() { return s_api; }
	static std::unique_ptr<RendererAPI> create();
private:
	static API s_api;
};
