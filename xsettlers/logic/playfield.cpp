//#include "Playfield.h"
//
//PlayField::PlayField(const std::string& map_filepath) {
//	loadMapFile(map_filepath);
//}
//
//PlayField::PlayField(uint32_t width, uint32_t height)
//	: m_width(width), m_height(height)
//{
//	m_walkable_mask = std::vector<std::vector<bool>>(width);
//	m_movables = std::vector<std::vector<Movable*>>(width);
//	for (uint32_t i = 0; i < width; i++) {
//		m_walkable_mask[i] = std::vector<bool>(height, true);
//		m_movables[i] = std::vector<Movable*>(height, nullptr);
//	}
//
//	// initialize play field border
//	for (uint32_t row = 0; row < m_height; row++) {
//		for (uint32_t col = 0; col < m_width; col++) {
//			if (col == 0 || col == m_width - 1 || row == 0 || row == m_height - 1) {
//				m_walkable_mask[col][row] = false;
//			}
//		}
//	}
//
//}
//
//void PlayField::Update() {
//	for (uint32_t y = 0; y < m_height; y++) {
//		UpdateMovablesRow(y);
//	}
//	m_tick++;
//}
//
//bool PlayField::PlaceMovable(glm::ivec2& pos, Movable* m) {
//	// TODO: Push idle Movable to side
//	if (m_walkable_mask[pos.y][pos.x] == true && m_movables[pos.y][pos.x] == nullptr) {
//		m->m_pos = pos;
//		m->m_steps = 0;
//		m->m_tick = m_tick;
//		m_movables[pos.y][pos.x] = m;
//		return true;
//	}
//	return false;
//}
//
//std::queue<glm::ivec2> PlayField::GetPath(glm::ivec2& start, glm::ivec2& goal) {
//	//TODO AStarJPS()
//}
//
//void PlayField::UpdateMovablesRow(uint32_t& y) {
//	for (uint32_t x = 0; x < m_width; x++)
//	{
//		if (m_movables[x][y] == nullptr) continue;
//
//		Movable* m = m_movables[x][y];
//
//		//m->m_lock.lock();
//		if (m->m_steps > 1) { // moving
//			m->m_steps -= m->m_speed;
//		}
//		else if (m->m_steps == 1) { // arriving
//			// unblock trailing fields
//			m_movables_mask[m->m_pos.x][m->m_pos.y] = false;
//			m_movables[m->m_pos.x][m->m_pos.y] = nullptr;
//			m->m_pos = m->m_tar;
//			m->m_steps = 0;
//			m_movables[m->m_pos.x][m->m_pos.y] = m;
//		}
//		else { // .. == 0
//			if (m->m_path->empty()) { // switch to idle and unblock current field
//				m_movables_mask[m->m_pos.x][m->m_pos.y] = false;
//			}
//			else { // start moving in next direction from path
//				// 1) get pos of targeted field
//				m->m_dir = Direction(m->m_pos, m->m_path->front());
//				m->m_path->pop();
//				m->m_tar = Step(m->m_pos, m->m_dir);
//
//				// 2) check if target field is blocked
//				if (m_movables_mask[m->m_tar.x][m->m_tar.y] == false) {
//					// not blocked
//					// 3.A) block field and start walking
//					m_movables_mask[m->m_tar.x][m->m_tar.y] = true;
//					m->m_steps = STEPS_PER_EDGE;
//				}
//				else {
//					// blocked
//					// 3.B) check if left/right are free
//					DIR right_dir = static_cast<DIR>((m->m_dir + 1) % DIR::COUNT);
//					glm::ivec2 right_tar = Step(m->m_pos, right_dir);
//
//					DIR left_dir = static_cast<DIR>((m->m_dir - 1) % DIR::COUNT);
//					glm::ivec2 left_tar = Step(m->m_pos, left_dir);
//
//					if (m_walkable_mask[right_tar.x][right_tar.y] == false) {
//						m_walkable_mask[right_tar.x][right_tar.y] = true;
//						m->m_dir = right_dir;
//						m->m_tar = right_tar;
//						m->m_steps = STEPS_PER_EDGE;
//						// >> calculate new or repair path here!
//					}
//					else if (m_walkable_mask[left_tar.x][left_tar.y] == false) {
//						m_walkable_mask[left_tar.x][left_tar.y] = true;
//						m->m_dir = left_dir;
//						m->m_tar = left_tar;
//						m->m_steps = STEPS_PER_EDGE;
//						// >> calculate new or repair path here!
//					}
//					else {
//						// just stand still for this round and cry.. 
//						//   \(;n; \)    \( ;A; )/    (/ ;n;)/
//					}
//				}
//			}
//		}
//		m->m_tick++;
//		//m->m_lock.unlock();
//	}
//}
//
//inline std::vector<glm::ivec2> PlayField::Neighbours(glm::ivec2 pos) {
//	std::vector<glm::ivec2> neighbours;
//	neighbours.reserve(6);
//	neighbours.emplace_back(glm::ivec2(pos.x - 1, pos.y - 1));
//	neighbours.emplace_back(glm::ivec2(pos.x, pos.y - 1));
//	neighbours.emplace_back(glm::ivec2(pos.x - 1, pos.y));
//	neighbours.emplace_back(glm::ivec2(pos.x + 1, pos.y));
//	neighbours.emplace_back(glm::ivec2(pos.x, pos.y + 1));
//	neighbours.emplace_back(glm::ivec2(pos.x + 1, pos.y + 1));
//	return neighbours;
//}
//
//inline glm::ivec2 PlayField::Step(glm::ivec2& pos, DIR dir) {
//	switch (dir) {
//	case DIR::NE: return glm::ivec2(pos.x, pos.y - 1);
//	case DIR::SW: return glm::ivec2(pos.x, pos.y + 1);
//	case DIR::SE: return glm::ivec2(pos.x + 1, pos.y + 1);
//	case DIR::NW: return glm::ivec2(pos.x - 1, pos.y - 1);
//	case DIR::W:  return glm::ivec2(pos.x - 1, pos.y);
//	case DIR::E:  return glm::ivec2(pos.x + 1, pos.y);
//	}
//}
//
//inline DIR PlayField::Direction(glm::ivec2& start, glm::ivec2& goal) {
//	if (start.x > goal.x) {
//		if (start.y == goal.y) return DIR::W;
//		if (start.y > goal.y) return DIR::NW;
//	}
//	else if (start.x < goal.x) {
//		if (start.y < goal.y) return DIR::SE;
//		if (start.y == goal.y) return DIR::E;
//	}
//	else {
//		if (start.y > goal.y) return DIR::NE;
//		if (start.y < goal.y) return DIR::SW;
//	}
//
//	if (start == goal) {
//		throw std::runtime_error("ERROR :: Direction() :: start and goal position are the same!");
//	}
//	else {
//		throw std::runtime_error("ERROR :: Direction() :: start and goal position aren't adjacent!");
//	}
//}
//
//// Map File Access
//
//void PlayField::loadMapFile(const std::string& map_filepath) {
//
//}
//
//// Path Finding
//
//typedef std::pair<double, glm::ivec2> prioNode;
//void PlayField::AStarJPS(glm::ivec2& start, glm::ivec2& goal,
//	std::unordered_map<glm::ivec2, glm::ivec2>& came_from,
//	std::unordered_map<glm::ivec2, int>& cost_so_far)
//{
//	std::priority_queue <prioNode, std::vector<prioNode>, std::greater<prioNode>> frontier;
//	std::vector<glm::ivec2> neighbours = Neighbours(start);
//	frontier.push(std::make_pair(0, start));
//
//	came_from[start] = start;
//	cost_so_far[start] = 0;
//
//	glm::ivec2 current = start;
//
//	while (!frontier.empty())
//	{
//		if (current == goal) {
//			break;
//		}
//
//		for (glm::ivec2 next : neighbours) {
//			// cost going from start to next
//			int new_cost = cost_so_far[current] + 1;
//
//			// if key not found OR cost is smaller that previous path to next
//			if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
//				cost_so_far[next] = new_cost;
//				came_from[next] = current;
//				double priority = new_cost + glm::distance(next, goal);
//				frontier.push(std::make_pair(priority, next));
//			}
//		}
//
//		current = frontier.top().second;
//		//neighbours = Neighbours(current);
//		neighbours = IdentifySuccessors(came_from[current], current, start, goal);
//	}
//}
//
//inline std::vector<glm::ivec2> PlayField::IdentifySuccessors(glm::ivec2& parent, glm::ivec2& x, glm::ivec2& start, glm::ivec2& goal) {
//	std::vector<glm::ivec2> successors;
//	std::vector<glm::ivec2> neighbours = PrunedNeighbours(parent, x);
//	for (glm::ivec2 n : neighbours) {
//		n = Jump(x, Direction(x, n), start, goal);
//		successors.push_back(n);
//	}
//	return successors;
//}
//
//inline glm::ivec2 PlayField::Jump(glm::ivec2& pos, DIR dir, glm::ivec2& start, glm::ivec2& goal) {
//	glm::ivec2 n = Step(pos, dir);
//	if (m_walkable_mask[n.x][n.y] == false) return glm::ivec2(0, 0);
//	if (n == goal) return n;
//
//	// if there is at least one "forced" neighbor (due to blocked areas)
//	std::vector<glm::ivec2> neighbours = Neighbours(pos);
//	if (neighbours.size() > 1) return n;
//
//	return Jump(n, dir, start, goal);
//}
//
//inline std::vector<glm::ivec2> PlayField::PrunedNeighbours(glm::ivec2& parent, glm::ivec2& x) {
//	std::vector<glm::ivec2> neighbours;
//
//	DIR dir = Direction(parent, x);
//
//	glm::ivec2 straight = Step(x, Direction(parent, x));
//	if (m_walkable_mask[straight.x][straight.y] == true) {
//		neighbours.push_back(straight);
//	}
//
//	glm::ivec2 left = Step(parent, static_cast<DIR>(dir + 1 % DIR::COUNT));
//	if (m_walkable_mask[left.x][left.y] == false) {
//		glm::ivec2 forced_left = Step(x, static_cast<DIR>(dir + 1 % DIR::COUNT));
//		if (m_walkable_mask[forced_left.x][forced_left.y] == true)
//			neighbours.push_back(forced_left);
//	}
//
//	glm::ivec2 right = Step(parent, static_cast<DIR>(dir - 1 % DIR::COUNT));
//	if (m_walkable_mask[right.x][right.y] == false) {
//		glm::ivec2 forced_right = Step(x, static_cast<DIR>(dir - 1 % DIR::COUNT));
//		if (m_walkable_mask[forced_right.x][forced_right.y] == true)
//			neighbours.push_back(forced_right);
//	}
//
//	return neighbours;
//}