#include "map_loader.h"
#include "platform/filesystem.h"
#include "core/log.h"
#include "logic/node.h"

MAPloader::MAPloader(const std::string filepath) {
	this->filepath_ = filepath;
	indexMapFile();
	readMapInfo();
};

void MAPloader::indexMapFile() {
	std::vector<char> map_file = readFile(filepath_);

	uint32_t part_type = 0;
	uint32_t part_length = 0;
	uint32_t part_offset = 8; // first part starts at 8th byte

	do {
		char* buffer = map_file.data();
		buffer += part_offset;

		memcpy(&part_type, buffer, sizeof(part_type));
		memcpy(&part_length, buffer + 4, sizeof(part_length));

		// decrypt
		part_type << 16;                        // bit shift
		part_type = part_type & 0x0000FFFF;     // masking
		part_length = part_length + 0xFFFFFFF8; // masking

		// check whether the part type is known
		if (part_type > 0 && part_type < PART_TYPE____COUNT) {
			std::cout << "Found Partition of Type \"" << part_type << "\"\nlength = " << part_length << "\noffset = " << part_offset + 8 << "\n" << std::endl;
			map_parts_[part_type].type = part_type;
			map_parts_[part_type].length = part_length;
			map_parts_[part_type].offset = part_offset + 8;
			map_parts_[part_type].encrypted = true;
		}

		part_offset += (8 + part_length);

	} while (part_type != 0);

	map_file.clear();
}

void MAPloader::readMapInfo() {
	part_info p_info = map_parts_[PART_TYPE_MapInfo];

	std::vector<char> map_file = readFile(filepath_);

	if (p_info.encrypted) decryptPartition(map_file, p_info);

	char* buffer = map_file.data();
	buffer += p_info.offset;

	int tmp = 0;
	memcpy(&tmp, buffer, sizeof(int));
	if (tmp <= 1) is_campaign_map_ = (tmp == 1);

	memcpy(&player_count_, buffer + 4, sizeof(int));
	memcpy(&resource_preset_, buffer + 8, sizeof(int));
	std::cout << "Players: " << player_count_ << "\nResources: " << resource_preset_ << "\n" << std::endl;

	map_file.clear();
}

void MAPloader::decryptPartition(std::vector<char>& map_file, part_info& p_info) {
	// bytewise decryption
	unsigned char key = p_info.type & 0xFF;
	char* buffer = map_file.data();
	buffer += p_info.offset;

	for (int i = 0; i < p_info.length; i++) {
		// take one byte
		unsigned char byt = *buffer ^ key; // bitwise XOR
		// overwrite old byte
		*buffer = byt;
		buffer++;
		// update key for followup with bitwise XOR
		key = ((key << 1) & 0xFF) ^ byt;
	}

	p_info.encrypted = false;
}

void MAPloader::loadMapArea(std::vector<Vertex>& vertex_buffer) {	
	part_info p_info = map_parts_[PART_TYPE_Area];
	std::vector<char> map_file = readFile(filepath_);

	if (p_info.encrypted) decryptPartition(map_file, p_info);

	char* buffer = map_file.data();
	buffer += p_info.offset;

	memcpy(&width_, buffer, sizeof(int));
	buffer += 4;

	uint32_t unit_count = (width_ - 1) * (width_ - 1);
	uint32_t vertex_count = width_ * width_;
	uint32_t triangle_count = (width_ - 1) * (width_ - 1) * 2;

	// load all vertices into a buffer
	std::vector<Vertex> vertices(vertex_count);
	std::vector<uint8_t> heights(vertex_count);
	for (int i = 0; i < vertex_count; i++) {
		uint8_t height = *buffer++;
		uint8_t type = *buffer++;
		uint8_t object = *buffer++; // ignore for now
		uint8_t player_claim = *buffer++; // ignore for now
		uint8_t accessible = *buffer++; // ignore for now
		uint8_t resources = *buffer++; // ignore for now

		Vertex v;
		v.pos = calcPixelPos(i);
		//v.type = type;

		vertices.at(i) = v;
		heights.at(i) = height;
	}

	map_file.clear();

	// use vertices of two adjacent triangles forming a rectangle
	//     0 --- 1
	//    / \ B /   (A) : 0, 3, 2
	//   / A \ /    (B) : 0, 1, 3
	//  2 --- 3

	std::vector<uint32_t> index_buffer;

	uint32_t i0 = 0;
	uint32_t i1 = 1;
	uint32_t i2 = i0 + width_;
	uint32_t i3 = i1 + width_;

	for (int i = 0; i < triangle_count; i += 2) {

		Vertex& v0 = vertices.at(i0);
		//Vertex& v1 = vertices.at(i1); // not needed for gradient calculation
		//Vertex& v2 = vertices.at(i2);
		//Vertex& v3 = vertices.at(i3);

		uint8_t h0 = heights.at(i0);
		uint8_t h2 = heights.at(i2);
		uint8_t h3 = heights.at(i3);

		// triangle A (clockwise) [0,3,2]
		index_buffer.push_back(i0);
		index_buffer.push_back(i3);
		index_buffer.push_back(i2);

		float g_color = calcGradient(h0, h2, h3);
		v0.shade = g_color;

		// triangle B (clockwise) [0,1,3]
		index_buffer.push_back(i0);
		index_buffer.push_back(i1);
		index_buffer.push_back(i3);

		// v1 is at rightmost edge, so jump to the next row
		if (((i1 + 1) % width_) == 0) {
			i0 += 2; i1 += 2; i2 += 2; i3 += 2;
		}
		else {
			i0++; i1++; i2++; i3++;
		}

	}

	// now that all vertices definitely have a gradient value attached to them
	// with the gradient applying for the underneath lying triangle A, continue
	// by filling the actual vertex buffer
	if (index_buffer.size() % 3 != 0) {
		throw std::runtime_error("index buffer doesn't adress the right amount of triangles!");
	}

	// XSTEP = 16 | 8 * XSTEP = 128
	// YSTEP = 9  | 8 * YSTEP =  72
	const float x_range = 128.0f; // range [0-128]x -> [0-1]u texture range
	const float y_range = 72.0f; // range [0-72]y -> [0-1]v texture range

	bool is_triangle_type_A = true;

	for (int i = 0; i < index_buffer.size(); i += 3) {

		uint32_t i0 = index_buffer.at(i + 0);
		uint32_t i1 = index_buffer.at(i + 1);
		uint32_t i2 = index_buffer.at(i + 2);

		Vertex vertex_0 = vertices.at(i0);
		Vertex vertex_1 = vertices.at(i1);
		Vertex vertex_2 = vertices.at(i2);

		float u0 = vertex_0.pos.x / x_range;
		float u1 = vertex_1.pos.x / x_range;
		float u2 = vertex_2.pos.x / x_range;

		float v0 = vertex_0.pos.y / y_range;
		float v1 = vertex_1.pos.y / y_range;
		float v2 = vertex_2.pos.y / y_range;

		//  \ |   \ |    <   \ |   \ |
		//  - 0 --- + -  >  - 0 --- 1 -
		//    | \   | \  <    | \ B | \
		//    |  \  |    >    |  \  |
		//  \ | A \ |    <  \ |   \ |
		//  - 2 --- 1 -  >  - + --- 2 -
		//    | \   | \  <    | \   | \

		// handle edge cases
		if (is_triangle_type_A) {
			// case 1 : 0v resolves to 1 from previous texture
			if (v0 > v1) v0 = 0.0f;

			// case 2 : 0u & 2u resolve to 1 from previous texture
			if (u0 > u1) u0 = u2 = 0.0f;

			is_triangle_type_A = false;
		}
		else {
			// case 1 : 0v & 1v resolve to 1 from previous texture
			if (v0 > v2) v0 = v1 = 0.0f;

			// case 2 : 0u resolves to 1 from previous texture
			if (u0 > u1) u0 = 0.0f;

			is_triangle_type_A = true;
		}

		//vertex_0.tex_uv = glm::vec2(u0, v0);
		//vertex_1.tex_uv = glm::vec2(u1, v1);
		//vertex_2.tex_uv = glm::vec2(u2, v2);

		// apply x_shift
		vertex_0.pos.x = vertex_0.pos.x + (width_ - 1 - vertex_0.pos.y / YSTEP) * (XSTEP / 2);
		vertex_1.pos.x = vertex_1.pos.x + (width_ - 1 - vertex_1.pos.y / YSTEP) * (XSTEP / 2);
		vertex_2.pos.x = vertex_2.pos.x + (width_ - 1 - vertex_2.pos.y / YSTEP) * (XSTEP / 2);

		// finally apply height shift
		vertex_0.pos.y = vertex_0.pos.y - heights.at(i0);
		vertex_1.pos.y = vertex_1.pos.y - heights.at(i1);
		vertex_2.pos.y = vertex_2.pos.y - heights.at(i2);

		vertex_buffer.push_back(vertex_0);
		vertex_buffer.push_back(vertex_1);
		vertex_buffer.push_back(vertex_2);
	}

}

glm::vec2 MAPloader::calcPixelPos(uint32_t index) {
	float x = (index % width_) * XSTEP;
	float y = (index / width_) * YSTEP;
	//float x_shift = (width_-1-y) * (XSTEP / 2);
	return glm::vec2(x, y);
}

float MAPloader::calcGradient(uint8_t top, uint8_t left, uint8_t right) {
	//--        [North]
	//--         top 
	//-- [West]  /  \  [East]
	//--        / A  \ 
	//--    left --- right  
	//--        [South]

	return .875f + (top - left) * .125f;

	enum enumGradient {
		GRADIENT_NONE,
		GRADIENT_NORTH,
		GRADIENT_NORTH_EAST,
		GRADIENT_EAST,
		GRADIENT_EAST_SOUTH,
		GRADIENT_SOUTH,
		GRADIENT_SOUTH_WEST,
		GRADIENT_WEST,
		GRADIENT_WEST_NORTH,
	};

	float GRADIENT_COLORS[] = { 1.0f, 0.7f, 0.7f, 1.0f, 1.0f, 1.3f, 1.3f, 1.0f, 1.0f };

	if ((top == left)) {
		if (left == right) return GRADIENT_COLORS[GRADIENT_NONE];       // .5
		if (top < right)  return GRADIENT_COLORS[GRADIENT_WEST_NORTH]; // .5
		if (right < top)  return GRADIENT_COLORS[GRADIENT_EAST_SOUTH]; // .5
	}

	if (left == right) {
		if (left < top) return GRADIENT_COLORS[GRADIENT_SOUTH]; // .7
		if (top < left) return GRADIENT_COLORS[GRADIENT_NORTH]; // .3
	}

	if (top == right) {
		if (top < left) return GRADIENT_COLORS[GRADIENT_NORTH_EAST]; // .3
		if (left < top) return GRADIENT_COLORS[GRADIENT_SOUTH_WEST]; // .7
	}

	if (right < left) {
		if (top < right) return GRADIENT_COLORS[GRADIENT_NORTH_EAST]; // .3
		if (left < top) return GRADIENT_COLORS[GRADIENT_EAST_SOUTH]; // .5
	}

	if (top < left) {
		if (right < top) return GRADIENT_COLORS[GRADIENT_EAST];       // .5
		if (left < right) return GRADIENT_COLORS[GRADIENT_WEST_NORTH]; // .5
	}

	if ((left < top) && (top < right)) return GRADIENT_COLORS[GRADIENT_WEST];       // .5
	if ((left < right) && (right < top)) return GRADIENT_COLORS[GRADIENT_SOUTH_WEST]; // .7

	return GRADIENT_COLORS[GRADIENT_NONE];

}
