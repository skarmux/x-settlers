#pragma once

#include "rendering/renderer_api.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	void init() override;
	void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	void set_clear_color(const glm::vec4& color) override;
	void clear() override;

	//void draw_indexed(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer, uint32_t count = 0) override;
	void draw_indexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	void draw(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0) override;
};
