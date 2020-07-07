#include "rendering/vertex_array.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_vertex_array.h"

std::shared_ptr<VertexArray> VertexArray::create()
{
	switch (Renderer2D::get_api())
	{
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		default: return nullptr;
	}
}