#include "platform/filesystem.h"
#include "rendering/renderer_2d.h"

#define STBI_ONLY_PNG

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" /* load image from file */

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"  /* write image to file */

void Filesystem::init()
{
	if (Renderer2D::get_api() == RendererAPI::API::OpenGL)
		stbi_set_flip_vertically_on_load(0);
}

std::vector<char> Filesystem::load_file(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("failed to open file!");

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

uint8_t* Filesystem::image_load(const std::string& filepath,
	uint32_t* width, uint32_t* height, uint32_t* channels, uint32_t forced_channels = 0)
{
	int _width, _height, _channels;
	stbi_uc* stbi_data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, (int)forced_channels);

	*width = (uint32_t)_width;
	*height = (uint32_t)_height;
	*channels = (uint32_t)_channels;

	return (uint8_t*)stbi_data;
}

void Filesystem::image_free(uint8_t* image)
{
	stbi_image_free((stbi_uc*)image);
}

void Filesystem::write_png(const std::string& path, uint32_t width, uint32_t height, uint32_t channels, uint8_t* data)
{
	stbi_write_png(path.c_str(), (int)width, height, (int)channels, data, (uint64_t)width * (uint64_t)channels * sizeof(uint8_t));
}

