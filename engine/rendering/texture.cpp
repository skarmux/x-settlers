#include "rendering/texture.h"
#include "rendering/renderer_2d.h"
#include "platform/opengl/opengl_texture.h"

std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
	switch (Renderer2D::GetAPI()) {
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(width, height);
	default: return nullptr;
	}
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
{
	switch (Renderer2D::GetAPI()) { 
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
	default: return nullptr;
	}
}