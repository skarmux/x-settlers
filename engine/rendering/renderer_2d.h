#pragma once

#include <glm/glm.hpp>

#include "rendering/renderer_api.h"
#include "rendering/texture.h"
#include "rendering/orthographic_camera.h"

class Renderer2D
{
public:
	static void init();
	static void shutdown();

	static void on_window_resize(uint32_t width, uint32_t height);

	static void begin_scene(const OrthographicCamera& camera);
	static void end_scene();
	static void flush();

	static void draw_quad(const glm::vec2& position, const std::shared_ptr<Texture2D>& texture);

	static void set_map(const uint32_t& size, const std::vector<uint8_t>& heights, const std::vector<uint8_t>& types);
	static void remove_map();

	//static void submit(const std::shared_ptr<VertexArray>& vertex_array, uint32_t triangle_vertex_count);

	static RendererAPI::API get_api() { return RendererAPI::get_api(); }

private:
	static void flush_and_reset();

};
