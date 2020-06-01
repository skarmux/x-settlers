#include "Shader.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
{
	switch (Renderer2D::GetAPI())
	{
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLShader>(filepath);
		case RendererAPI::API::Vulkan:  //return CreateRef<OpenGLShader>(filepath);
		default: return nullptr;
	}
}
