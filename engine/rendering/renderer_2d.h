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

	static void draw_tilemap(
		glm::vec2 position, // center aligned
		glm::vec2 size,
		const std::shared_ptr<Texture2D> atlas,
		const std::shared_ptr<Texture2D> tilemap,
		const std::shared_ptr<Texture2D> heightmap);

	static void draw_quad(
		const glm::vec2 position, // bottom/center aligned
		const std::shared_ptr<Texture2D> texture);

	static RendererAPI::API get_api() { return RendererAPI::get_api(); }

private:
	static void flush_and_reset();
	static void assign_texture_slot(const std::shared_ptr<Texture2D>& texture, uint32_t* tex_index);
};