#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <glm/glm.hpp>

#include "Logic/Playfield.h"
#include "Renderer/Vertex.h"

class MAPloader {
private:
	const uint8_t XSTEP = 16; // don't ever touch!
	const uint8_t YSTEP = 9;  // don't ever touch!

	enum part_type {
		PART_TYPE_EOF = 0, // End of File and Padding
		PART_TYPE_MapInfo = 1,
		PART_TYPE_PlayerInfo = 2,
		PART_TYPE_TeamInfo = 3,
		PART_TYPE_Preview = 4,
		PART_TYPE_Area = 6,
		PART_TYPE_Settlers = 7,
		PART_TYPE_Buildings = 8,
		PART_TYPE_Resources = 9,
		PART_TYPE_QuestText = 11,
		PART_TYPE_QuestTip = 12,
		PART_TYPE____COUNT
	};

	struct part_info {
		uint32_t type;
		uint32_t length;
		uint32_t offset;
		bool encrypted;
	};

	bool is_campaign_map_;

	int player_count_;
	int resource_preset_;
	int width_;

	//bool test[5][5];

	std::string filepath_;

	part_info map_parts_[PART_TYPE____COUNT];

	void indexMapFile();
	void readMapInfo();
	void decryptPartition(std::vector<char>& map_file, part_info& p_info);

public:
	MAPloader(const std::string);
	
	void loadMapArea(std::vector<Vertex>& vertices);
	glm::vec2 calcPixelPos(uint32_t index);
	float calcGradient(uint8_t h0, uint8_t h1, uint8_t h2);

};
