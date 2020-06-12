#pragma once

#include <memory>

class GraphicsContext
{
public:
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
	virtual void SetSwapInterval(int interval) = 0;

	static std::unique_ptr<GraphicsContext> Create(void* window);
};
