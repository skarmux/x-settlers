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
	inline glm::vec2 gridpos_to_worldpos(uint32_t x, uint32_t y, uint32_t width) const;
	glm::vec2* tex_coords_from_types(
		uint8_t type_0, uint8_t type_1, uint8_t type_2,
		const glm::ivec2& gridpos_0, const glm::ivec2& gridpos_1, const glm::ivec2& gridpos_2) const;
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