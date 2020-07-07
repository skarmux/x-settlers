#pragma once

class GraphicsContext
{
public:
	virtual void init() = 0;
	virtual void swap_buffers() = 0;
	virtual void set_swap_interval(int interval) = 0;

	static std::unique_ptr<GraphicsContext> create(void* window);
};
