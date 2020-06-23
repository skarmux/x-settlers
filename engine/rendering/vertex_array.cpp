#include "rendering/vertex_array.h"
#include "rendering/renderer_2d.h"
#include "platform/opengl/opengl_vertex_array.h"

std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (Renderer2D::GetAPI()) {
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		default: return nullptr;
	}
}