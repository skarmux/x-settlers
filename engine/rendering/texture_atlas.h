#pragma once

#include "rendering/texture.h"

class TextureAtlas : public Texture
{
public:
	virtual void add_texture(const Texture& texture) = 0;

	static std::shared_ptr<TextureAtlas> create(uint32_t width, uint32_t height);
};
