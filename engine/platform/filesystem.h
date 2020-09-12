#pragma once

#include <vector>
#include <string>
#include <fstream>

class Filesystem
{
public:
	static void init();

	static std::vector<char> load_file(const std::string& filepath);
	static uint8_t* image_load(const std::string& filepath, 
		uint32_t* width, uint32_t* height, uint32_t* channels, uint32_t forced_channels);
	static void image_free(uint8_t* image);
	static void write_png(const std::string& path, uint32_t width, uint32_t height, uint32_t channels, uint8_t* data);
};