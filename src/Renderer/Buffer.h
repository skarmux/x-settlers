#pragma once

#include <glm/glm.hpp>
#include <memory>

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	//virtual void SetData(const void* data, uint32_t size) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	static std::shared_ptr<VertexBuffer> Create(uint32_t size);
	static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() {}

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t size);
};
