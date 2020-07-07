#include "rendering/texture_atlas.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_texture_atlas.h"

std::shared_ptr<TextureAtlas> TextureAtlas::create(uint32_t width, uint32_t height)
{
	switch (Renderer2D::get_api())
	{
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTextureAtlas>(width, height);
	default: return nullptr;
	}
}