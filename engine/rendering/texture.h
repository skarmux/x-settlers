#pragma once

class Texture
{
public:
	enum class Format { RED = 1, RG = 2, RGB = 3, RGBA = 4 };
	enum class Type { FLOAT, BYTE, UNSIGNED_BYTE, SHORT, INT, UNSIGNED_INT };
	enum class Wrap { CLAMP, REPEAT };
	enum class Filter { NEAREST, LINEAR };
public:
	virtual ~Texture() = default;

	virtual void push_data(uint8_t* data,
		uint32_t width, uint32_t height, 
		Texture::Format comp, Texture::Type type) = 0;

	virtual void push_data(uint8_t* data, 
		uint32_t x, uint32_t y, 
		uint32_t width, uint32_t height, 
		Texture::Format comp, Texture::Type type) = 0;

	virtual void generate_mipmaps() = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;

	virtual void set_filter(Filter filter) = 0;
	virtual void set_wrapping(Wrap wrap) = 0;

	static std::shared_ptr<Texture2D> create(const std::string& path, 
		Texture::Format comp = Texture::Format::RGBA,
		Texture::Type type = Texture::Type::UNSIGNED_BYTE);

	static std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height, 
		Texture::Format comp = Texture::Format::RGBA,
		Texture::Type type = Texture::Type::UNSIGNED_BYTE);
};
