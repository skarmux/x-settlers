#include "platform/opengl/opengl_shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
	std::string source = read_file(filepath);
	RENDERER_INFO("Building shader from source: {0}", filepath);
	auto shaderSources = pre_process(source);
	compile(shaderSources);

	// Extract name from filepath
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& path, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	compile(sources);

	m_Name = path;
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_renderer_id);
}

// TODO: Move this utility function to Core/FileIO.h
std::string OpenGLShader::read_file(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != -1)
		{
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], size);
			in.close();
		}
		else
		{
			CORE_ERROR("Could not read from file '{0}'", filepath);
		}
	}
	else
	{
		CORE_ERROR("Could not open file '{0}'", filepath);
	}

	return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::pre_process(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
		//HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
		std::string type = source.substr(begin, eol - begin);
		//HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
		//HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
		pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

		shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
	}

	return shaderSources;
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	GLuint program = glCreateProgram();
	std::array<GLenum, 2> glShaderIDs;
	int glShaderIDIndex = 0;
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			CORE_ERROR("{0}", infoLog.data());
			CORE_ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs[glShaderIDIndex++] = shader;
	}

	m_renderer_id = program;

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		for (auto id : glShaderIDs)
			glDeleteShader(id);

		CORE_ERROR("{0}", infoLog.data());
		CORE_ASSERT(false, "Shader link failure!");
		return;
	}

	for (auto id : glShaderIDs)
	{
		glDetachShader(program, id);
		glDeleteShader(id);
	}
}

void OpenGLShader::bind() const
{
	glUseProgram(m_renderer_id);
}

void OpenGLShader::unbind() const
{
	glUseProgram(0);
}

void OpenGLShader::set_int(const std::string& path, int value)
{
	upload_uniform_int(path, value);
}

void OpenGLShader::set_int_array(const std::string& path, int* values, uint32_t count)
{
	upload_uniform_int_array(path, values, count);
}

void OpenGLShader::set_float(const std::string& path, float value)
{
	upload_uniform_float(path, value);
}

void OpenGLShader::set_float3(const std::string& path, const glm::vec3& value)
{
	upload_uniform_float3(path, value);
}

void OpenGLShader::set_float4(const std::string& path, const glm::vec4& value)
{
	upload_uniform_float4(path, value);
}

void OpenGLShader::set_mat4(const std::string& path, const glm::mat4& value)
{
	upload_uniform_mat4(path, value);
}

void OpenGLShader::upload_uniform_int(const std::string& path, int value)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_int_array(const std::string& path, int* values, uint32_t count)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform1iv(location, count, values);
}

void OpenGLShader::upload_uniform_float(const std::string& path, float value)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float2(const std::string& path, const glm::vec2& value)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform2f(location, value.x, value.y);
}

void OpenGLShader::upload_uniform_float3(const std::string& path, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::upload_uniform_float4(const std::string& path, const glm::vec4& value)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::upload_uniform_mat3(const std::string& path, const glm::mat3& matrix)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::upload_uniform_mat4(const std::string& path, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(m_renderer_id, path.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
