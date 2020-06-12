#pragma once

#include "rendering/graphics_context.h"

#include <EGL/egl.h>

class GLESContext : public GraphicsContext
{
public:
	GLESContext(EGLSurface* m_SurfaceHandle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
	virtual void SetSwapInterval(int interval) override;

private:
	EGLDisplay* m_DisplayHandle;
    EGLSurface* m_SurfaceHandle;
	EGLContext* m_ContextHandle;

	//std::unique_ptr<GraphicsContext> m_ContextHandle;
};