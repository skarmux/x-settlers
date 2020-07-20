#pragma once

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void push_data(void* data, size_t size) = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;

	static std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height);
	static std::shared_ptr<Texture2D> create(const std::string& path);
};

class Texture3D : public Texture
{
public:
	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;
	virtual uint32_t get_depth() const = 0;

	static std::shared_ptr<Texture3D> create(uint32_t width, uint32_t height, uint32_t depth, uint32_t channels, uint32_t bit_depth);
	static std::shared_ptr<Texture3D> create(const std::vector<std::string>& paths);
};

//class BufferTexture : public Texture
//{
//public:
//	static std::shared_ptr<BufferTexture> create(uint32_t size);
//};