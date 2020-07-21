//#pragma once
//
//#include "movable.h"
//#include "core/logger.h"
//
//#include <mutex>
//#include <vector>
//#include <queue>
//#include <unordered_map>
//#include <iostream>
//#include <glm/glm.hpp>
//
//static const uint8_t STEPS_PER_EDGE = 10;
//
//class PlayField
//{
//public:
//	PlayField(const std::string& map_filepath);
//	PlayField(uint32_t width, uint32_t height);
//	virtual ~PlayField();
//
//	void on_update();
//	bool place_movable(glm::ivec2& pos, Movable* m);
//	std::queue<glm::ivec2> get_path(glm::ivec2& start, glm::ivec2& goal);
//
//private:
//	void update_movables_row(uint32_t& row);
//	inline std::vector<glm::ivec2> neighbours(glm::ivec2 pos);
//	inline glm::ivec2 step(glm::ivec2& pos, Direction dir);
//	inline Direction direction(glm::ivec2& start, glm::ivec2& goal);
//	
//	void load_map_file(const std::string& map_filepath);
//
//	void a_star_jps(glm::ivec2& start, glm::ivec2& goal, std::unordered_map<glm::ivec2, glm::ivec2>& came_from, std::unordered_map<glm::ivec2, int>& cost_so_far);
//	inline std::vector<glm::ivec2> identify_successors(glm::ivec2& parent, glm::ivec2& x, glm::ivec2& start, glm::ivec2& goal);
//	inline glm::ivec2 jump(glm::ivec2& pos, Direction dir, glm::ivec2& start, glm::ivec2& goal);
//	inline std::vector<glm::ivec2> pruned_neighbours(glm::ivec2& parent, glm::ivec2& x);
//
//private:
//	uint32_t m_tick;
//	uint32_t m_width;
//	uint32_t m_height;
//	std::vector<std::vector<bool>> m_walkable_mask; // For Path finding
//	std::vector<std::vector<bool>> m_movables_mask; // For Agent Interaction
//	std::vector<std::vector<Movable*>> m_movables;
//};
