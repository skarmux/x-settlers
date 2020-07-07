#pragma once

#include "renderer_api.h"

#include <glm/glm.hpp>

class RenderCommand
{
public:
	static void init()
	{
		s_renderer_api->init();
	}

	static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_renderer_api->set_viewport(x, y, width, height);
	}

	static void set_clear_color(const glm::vec4& color)
	{
		s_renderer_api->set_clear_color(color);
	}

	static void clear()
	{
		s_renderer_api->clear();
	}

	static void draw(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0)
	{
		s_renderer_api->draw(vertex_array, count);
	}
	
	//virtual void draw_indexed(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) = 0;
	
	/*static void draw_indexed(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer, uint32_t count = 0)
	{
		s_renderer_api->draw_indexed(vertex_buffer, index_buffer, count);
	}*/

	static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0)
	{
		s_renderer_api->draw_indexed(vertex_array, index_count);
	}

private:
	static std::unique_ptr<RendererAPI> s_renderer_api;
};
