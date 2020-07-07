#pragma once

#include "rendering/shader.h"

#include <glad/glad.h>

class OpenGLShader : public Shader
{
public:
	OpenGLShader(const std::string& filepath);
	OpenGLShader(const std::string& path, const std::string& vertexSrc, const std::string& fragmentSrc);
	~OpenGLShader();

	void bind() const override;
	void unbind() const override;

	// set uniforms
	void set_int(const std::string& path, int value) override;
	void set_int_array(const std::string& path, int* values, uint32_t count) override;
	void set_float(const std::string& path, float value) override;
	void set_float3(const std::string& path, const glm::vec3& value) override;
	void set_float4(const std::string& path, const glm::vec4& value) override;
	void set_mat4(const std::string& path, const glm::mat4& value) override;

	const std::string& get_name() const override { return m_Name; }

	// pass uniforms
	void upload_uniform_int(const std::string& path, int value);
	void upload_uniform_int_array(const std::string& path, int* values, uint32_t count);
	void upload_uniform_float(const std::string& path, float value);
	void upload_uniform_float2(const std::string& path, const glm::vec2& value);
	void upload_uniform_float3(const std::string& path, const glm::vec3& value);
	void upload_uniform_float4(const std::string& path, const glm::vec4& value);
	void upload_uniform_mat3(const std::string& path, const glm::mat3& matrix);
	void upload_uniform_mat4(const std::string& path, const glm::mat4& matrix);
private:
	std::string read_file(const std::string& filepath); // TODO move to filesystem module
	std::unordered_map<GLenum, std::string> pre_process(const std::string& source);
	void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
	uint32_t m_renderer_id;
	std::string m_Name;
};
