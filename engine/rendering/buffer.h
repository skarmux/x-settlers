#pragma once

enum class ShaderDataType
{
	None = 0, Float, Vec2, Vec3, Vec4, Mat3, Mat4, Int, UVec2, UVec3, UVec4, Bool
};

static uint32_t sizeof_shader_data_type(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Vec2:  return 4 * 2;
		case ShaderDataType::Vec3:  return 4 * 3;
		case ShaderDataType::Vec4:  return 4 * 4;
		case ShaderDataType::Mat3:  return 4 * 3 * 3;
		case ShaderDataType::Mat4:  return 4 * 4 * 4;
		case ShaderDataType::Int:   return 4;
		case ShaderDataType::UVec2: return 4 * 2;
		case ShaderDataType::UVec3: return 4 * 3;
		case ShaderDataType::UVec4: return 4 * 4;
		case ShaderDataType::Bool:  return 1;				   
	}
	return 0;
}

struct BufferElement
{
	std::string path;
	ShaderDataType type;
	uint32_t size;
	size_t offset;
	bool normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string& path, size_t offset = 0, bool normalized = false)
		: path(path), type(type), size(sizeof_shader_data_type(type)), offset(offset), normalized(normalized)
	{}

	uint32_t get_component_count() const
	{
		switch (type)
		{
		case ShaderDataType::Float:  return 1;
		case ShaderDataType::Vec2:  return 2;
		case ShaderDataType::Vec3:  return 3;
		case ShaderDataType::Vec4:  return 4;
		case ShaderDataType::Mat3:    return 3; // 3 * Float3
		case ShaderDataType::Mat4:    return 4; // 4 * Float4
		case ShaderDataType::Int:    return 1;
		case ShaderDataType::UVec2:    return 2;
		case ShaderDataType::UVec3:    return 3;
		case ShaderDataType::UVec4:    return 4;
		case ShaderDataType::Bool:    return 1;			  
		}
		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {}

	BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_elements(elements)
	{
		calculate_offsets_and_stride();
	}

	uint32_t get_stride() const { return m_stride; }
	const std::vector<BufferElement>& get_elements() const { return m_elements; }

	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }

	std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
private:
	void calculate_offsets_and_stride()
	{
		size_t offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}
private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride = 0;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void push_data(const void* data, size_t size) = 0;

	virtual const BufferLayout& get_layout() const = 0;
	virtual void set_layout(const BufferLayout& layout) = 0;

	static std::shared_ptr<VertexBuffer> create(size_t size);
	static std::shared_ptr<VertexBuffer> create(float* vertices, size_t size);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() {}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual uint32_t get_count() const = 0;

	static std::shared_ptr<IndexBuffer> create(uint32_t* indices, uint32_t count);
};