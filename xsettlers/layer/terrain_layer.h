#pragma once

#include <engine.h>

#include "logic/map_loader.h"
#include "rendering/renderer_2d.h"

enum class S3Field
{
	Sea0 = 0, Sea1 = 1, Sea2 = 2, Sea3 = 3, 
	Sea4 = 4, Sea5 = 5, Sea6 = 6, Sea7 = 7,

	Grass = 16, GrassRock = 17, GrassRiver = 19, GrassDesert = 20,
	GrassSwamp = 21, GrassMud = 23,

	Rock = 32, RockGrass = 33, RockSnow = 35,

	Beach = 48, BeachRiver = 49,

	Desert = 64, DesertGrass = 65, DesertBeach0 = 66, DesertBeach1 = 67,

	Swamp = 80, SwampGrass = 81, SwampRock = 83,
	
	River0 = 96, River1 = 97, River2 = 98, River3 = 99,
	
	Snow = 128, SnowRock = 129,

	Mud = 144, MudGrass = 145
};

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