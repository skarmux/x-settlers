#include "rendering/shader.h"
#include "rendering/renderer_2d.h"

#include "platform/opengl/opengl_shader.h"

std::shared_ptr<Shader> Shader::create(const std::string& filepath)
{
	switch (Renderer2D::get_api())
	{
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLShader>(filepath);
		default: return nullptr;
	}
}

std::shared_ptr<Shader> Shader::create(const std::string& path, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer2D::get_api())
	{
	case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLShader>(path, vertexSrc, fragmentSrc);
	default: return nullptr;
	}
}
