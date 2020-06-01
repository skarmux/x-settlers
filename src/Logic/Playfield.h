#pragma once

#include "Movable.h"
#include "../Core/Log.h"

#include <mutex>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <glm/glm.hpp>

static const uint8_t STEPS_PER_EDGE = 10;

class PlayField {
public:
	PlayField(const std::string& map_filepath);
	PlayField(uint32_t width, uint32_t height);
	virtual ~PlayField();

	void Update();
	bool PlaceMovable(glm::ivec2& pos, Movable* m);
	std::queue<glm::ivec2> GetPath(glm::ivec2& start, glm::ivec2& goal);

private:
	void UpdateMovablesRow(uint32_t& row);
	inline std::vector<glm::ivec2> Neighbours(glm::ivec2 pos);
	inline glm::ivec2 Step(glm::ivec2& pos, DIR dir);
	inline DIR Direction(glm::ivec2& start, glm::ivec2& goal);
	
	void loadMapFile(const std::string& map_filepath);

	void AStarJPS(glm::ivec2& start, glm::ivec2& goal, std::unordered_map<glm::ivec2, glm::ivec2>& came_from, std::unordered_map<glm::ivec2, int>& cost_so_far);
	inline std::vector<glm::ivec2> IdentifySuccessors(glm::ivec2& parent, glm::ivec2& x, glm::ivec2& start, glm::ivec2& goal);
	inline glm::ivec2 Jump(glm::ivec2& pos, DIR dir, glm::ivec2& start, glm::ivec2& goal);
	inline std::vector<glm::ivec2> PrunedNeighbours(glm::ivec2& parent, glm::ivec2& x);

private:
	uint32_t m_tick;
	uint32_t m_width;
	uint32_t m_height;
	std::vector<std::vector<bool>> m_walkable_mask; // For Path finding
	std::vector<std::vector<bool>> m_movables_mask; // For Agent Interaction
	std::vector<std::vector<Movable*>> m_movables;
};
