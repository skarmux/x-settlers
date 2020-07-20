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
	MapNode* m_map_area;
	MapInfo m_map_info; // TODO: may be removable
	OrthographicCameraController m_camera_controller;

	// Store Vertex Information (TODO: Quadtree)
	Renderer2D::TriVertex* m_vertex_buffer;

	// Textures
	std::vector<std::shared_ptr<Texture2D>> m_tex_terrain;
	//std::shared_ptr<Texture2D> m_test_settler;
};