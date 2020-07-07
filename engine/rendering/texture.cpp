#include "rendering/texture.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_texture.h"

std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
{
	switch (Renderer2D::get_api())
	{
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(width, height);
	default: return nullptr;
	}
}

std::shared_ptr<Texture2D> Texture2D::create(const std::string& path)
{
	switch (Renderer2D::get_api())
	{ 
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
	default: return nullptr;
	}
}