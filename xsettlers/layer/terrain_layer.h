#pragma once

#include <engine.h>

#include "logic/map_loader.h"
#include "rendering/renderer_2d.h"

class TerrainLayer : public Layer
{
public:
	TerrainLayer(uint32_t width, uint32_t height, MapNode* nodes, uint32_t map_size) :
		Layer(), m_nodes(nodes), m_map_size(map_size),
		m_camera_controller(width, height) {}
	~TerrainLayer() = default;

	void on_attach() override;
	void on_detach() override;

	void on_update(TimeDelta ts) override;
	void on_event(Event& e) override;
private:
	MapNode* const m_nodes;
	uint32_t m_map_size;

	OrthographicCameraController m_camera_controller;

	std::shared_ptr<Texture2D> m_atlas, m_tilemap, m_heightmap;
};