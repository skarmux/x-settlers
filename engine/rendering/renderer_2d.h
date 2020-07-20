#pragma once

#include <glm/glm.hpp>

#include "rendering/renderer_api.h"
#include "rendering/texture.h"
#include "rendering/orthographic_camera.h"

static const glm::vec2 QUAD_TEX_COORDS[] =
	{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

class Renderer2D
{
public:
	struct TriVertex
	{
		glm::vec2 pos;
		glm::vec2 tex;
		float tex_index;
		float shade;
	};

	struct QuadVertex
	{
		glm::vec2 pos;
		glm::vec2 tex;
		float tex_index;
	};

	struct RhombVertex
	{
		glm::vec2 pos;
		glm::vec2 tex;
		float col;
		float tex_index;
	};
public:
	static void init();
	static void shutdown();

	static void on_window_resize(uint32_t width, uint32_t height);

	static void begin_scene(const OrthographicCamera& camera);
	static void end_scene();
	static void flush();

	static void draw_triangle(
		const glm::vec2 positions[3],
		const glm::vec2 tex_coords[3],
		const float colors[3],
		const std::shared_ptr<Texture2D>& texture);

	static void draw_quad(
		const glm::vec2 position, // bottom/center aligned
		const std::shared_ptr<Texture2D>& texture,
		const glm::vec2 tex_coords[3] = QUAD_TEX_COORDS);

	static void draw_rhomboid(
		const glm::vec2 position, // bottom/left vertex
		const float top_shift,
		const std::shared_ptr<Texture2D>& texture,
		const glm::vec2* tex_coords = QUAD_TEX_COORDS);

	static void submit(TriVertex* buffer, uint32_t count, const std::shared_ptr<Texture2D>& texture);

	static RendererAPI::API get_api() { return RendererAPI::get_api(); }

private:
	static void flush_and_reset();
	static void assign_texture_slot(const std::shared_ptr<Texture2D>& texture, uint32_t& tex_index);
};