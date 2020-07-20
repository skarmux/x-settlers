#pragma once

#include <glm/glm.hpp>

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	// set uniforms
	virtual void set_int(const std::string& path, int value) = 0;

	virtual void set_float(const std::string& path, float value) = 0;
	virtual void set_vec2(const std::string& path, const glm::vec2& value) = 0;
	virtual void set_vec3(const std::string& path, const glm::vec3& value) = 0;
	virtual void set_vec4(const std::string& path, const glm::vec4& value) = 0;

	virtual void set_int_array(const std::string& path, int* values, uint32_t count) = 0;
	virtual void set_float_array(const std::string& path, float* values, uint32_t count) = 0;

	virtual void set_mat3(const std::string& path, const glm::mat3& value) = 0;
	virtual void set_mat4(const std::string& path, const glm::mat4& value) = 0;

	virtual const std::string& get_name() const = 0;

	static std::shared_ptr<Shader> create(const std::string& filepath);
	static std::shared_ptr<Shader> create(const std::string& path, const std::string& vertexSrc, const std::string& fragmentSrc);
};