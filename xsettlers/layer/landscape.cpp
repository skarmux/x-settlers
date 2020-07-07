#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "layer/landscape.h"
#include "logic/map_loader.h"

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

Landscape::Landscape() :
	Layer(), m_camera_controller( (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT )
{}

void Landscape::on_attach()
{
	RenderCommand::set_clear_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
	m_grass_texture = Texture2D::create("assets/textures/grass.png");
	m_grass_texture->bind();

	//m_camera_controller.set_position(glm::vec3(64.0f, 128.0f, 0.0f));
	m_camera_controller.set_position(glm::vec3(5376.0f, 2016.0f, 0.0f));

	MapLoader::init();

	std::vector<MapInfo>& maps = MapLoader::get_map_list();
	for (MapInfo d : maps) {
		LOGIC_INFO("loaded map {0}: width {1}, campaign ? {2}, players {3}, resources {4}",
			d.path, d.size, d.is_campaign, d.player_count, d.resource_preset
		);
	}

	MapArea map_area;
	MapLoader::load_map_area(0, map_area);
	Renderer2D::set_map(maps[0].size, map_area.heights, map_area.types);
}

void Landscape::on_detach()
{
	// ...
}

void Landscape::on_update(TimeDelta time_delta)
{
	m_camera_controller.on_update(time_delta);

	RenderCommand::clear();

	{
		Renderer2D::begin_scene(m_camera_controller.get_camera());
		Renderer2D::draw_quad(glm::vec2(-2.0f, -2.0f), m_grass_texture);
		Renderer2D::draw_quad(glm::vec2(130.0f, -2.0f), m_grass_texture);
		Renderer2D::draw_quad(glm::vec2(130.0f, 130.0f), m_grass_texture);
		Renderer2D::draw_quad(glm::vec2(-2.0f, 130.0f), m_grass_texture);
		Renderer2D::end_scene();
	}
}

void Landscape::on_event(Event& e)
{
	m_camera_controller.on_event(e);
}