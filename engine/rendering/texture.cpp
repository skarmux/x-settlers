#include "rendering/texture.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_texture.h"
#include "platform/filesystem.h"

std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height, Texture::Format comp, Texture::Type type)
{
	std::shared_ptr<Texture2D> texture = nullptr;

	switch (Renderer2D::get_api()) {
	case RendererAPI::API::OpenGL: 
		texture = std::make_shared<OpenGLTexture2D>(width, height, comp, type); break;
	}

	return texture;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::string& path, Texture::Format comp, Texture::Type type)
{
	std::shared_ptr<Texture2D> texture = nullptr;

	uint32_t width, height, channels;
	uint8_t* data_ptr = Filesystem::image_load(path.c_str(), &width, &height, &channels, static_cast<uint32_t>(comp));

	switch (Renderer2D::get_api()) {
	case RendererAPI::API::OpenGL:
		texture = std::make_shared<OpenGLTexture2D>(width, height, comp, type); break;
	default:
		return texture;
	}

	texture->push_data(data_ptr, width, height, comp, type);

	Filesystem::image_free(data_ptr);

	return texture;
}