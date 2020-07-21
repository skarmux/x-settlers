#pragma once

#include <engine.h>

#include "logic/map_loader.h"
#include "rendering/renderer_2d.h"

class TerrainLayer : public Layer
{
public:
	TerrainLayer(uint32_t width, uint32_t height) :
		Layer(), 
		m_camera_controller(width, height),
		m_map_area(nullptr),
		m_vertex_buffer(nullptr) {}
	~TerrainLayer() = default;

	void on_attach() override;
	void on_detach() override;

	void on_update(TimeDelta ts) override;
	void on_event(Event& e) override;
private:
	glm::vec2* tex_coords_from_types(
		uint8_t type_a, uint8_t type_b, uint8_t type_c,
		const glm::vec2& pos_a, const glm::vec2& pos_b, const glm::vec2& pos_c) const;
private:
	MapNode* m_map_area;
	MapInfo m_map_info; // TODO: may be removable
	OrthographicCameraController m_camera_controller;

	// Store Vertex Information (TODO: Quadtree)
	Renderer2D::TriVertex* m_vertex_buffer;

	// Textures
	TextureAtlas m_atlas;
	std::shared_ptr<Texture2D> m_atlas_texture;
	//std::vector<std::shared_ptr<Texture2D>> m_tex_terrain;
};