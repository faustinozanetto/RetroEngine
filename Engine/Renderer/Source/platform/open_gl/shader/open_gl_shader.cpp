#include "pch.h"

#include "open_gl_shader.h"
#include "core/application/retro_application.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Retro::Renderer
{
	open_gl_shader::open_gl_shader(const std::string& vertexPath, const std::string& fragmentPath) : asset(
		asset_type::shader)
	{
		logger::line();
		// Parse both vertex and fragment shaders sources from file.
		const std::string& vertexSource = parse_shader_contents_from_file(vertexPath);
		const std::string& fragmentSource = parse_shader_contents_from_file(fragmentPath);
		// Assign shader sources.
		m_shader_sources = process_shader_sources(vertexSource, fragmentSource);
		// Compile shader.
		compile_shader();
		logger::line();
	}

	open_gl_shader::~open_gl_shader()
	{
		glDeleteProgram(m_object_handle);
	}

	void open_gl_shader::bind()
	{
		glUseProgram(m_object_handle);
	}

	void open_gl_shader::un_bind()
	{
		glUseProgram(0);
	}

	void open_gl_shader::set_float(const std::string& uniform, float value)
	{
		glUniform1f(get_uniform_location(uniform), value);
	}

	void open_gl_shader::set_vec_float2(const std::string& uniform, const glm::vec2& value)
	{
		glUniform2f(get_uniform_location(uniform), value.x, value.y);
	}

	void open_gl_shader::set_vec_float3(const std::string& uniform, const glm::vec3& value)
	{
		glUniform3f(get_uniform_location(uniform), value.x, value.y, value.z);
	}

	void open_gl_shader::set_vec_float4(const std::string& uniform, const glm::vec4& value)
	{
		glUniform4f(get_uniform_location(uniform), value.x, value.y, value.z, value.w);
	}

	void open_gl_shader::set_int(const std::string& uniform, int value)
	{
		glUniform1i(get_uniform_location(uniform), value);
	}

	void open_gl_shader::set_vec_int2(const std::string& uniform, const glm::ivec2& value)
	{
		glUniform2i(get_uniform_location(uniform), value.x, value.y);
	}

	void open_gl_shader::set_vec_int3(const std::string& uniform, const glm::ivec3& value)
	{
		glUniform3i(get_uniform_location(uniform), value.x, value.y, value.z);
	}

	void open_gl_shader::set_vec_int4(const std::string& uniform, const glm::ivec4& value)
	{
		glUniform4i(get_uniform_location(uniform), value.x, value.y, value.z, value.w);
	}

	void open_gl_shader::set_mat3(const std::string& uniform, const glm::mat3& value)
	{
		glUniformMatrix3fv(get_uniform_location(uniform), 1, GL_FALSE, value_ptr(value));
	}

	void open_gl_shader::set_mat4(const std::string& uniform, const glm::mat4& value)
	{
		glUniformMatrix4fv(get_uniform_location(uniform), 1, GL_FALSE, value_ptr(value));
	}

	int open_gl_shader::get_uniform_location(const std::string& uniform)
	{
		if (m_uniform_locations.contains(uniform))
		{
			return m_uniform_locations[uniform];
		}
		// Get uniform location.
		const int location = glGetUniformLocation(m_object_handle, uniform.c_str());
		// Store uniform location in map.
		m_uniform_locations[uniform] = location;
		return location;
	}

	std::string open_gl_shader::parse_shader_contents_from_file(const std::string& shader_file_path)
	{
		logger::info("open_gl_shader::parse_shader_contents_from_file | Parsing: " + shader_file_path);
		std::string parsedContents;
		std::ifstream in(shader_file_path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			const size_t size = in.tellg();
			// Valid size.
			if (size != -1)
			{
				parsedContents.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(parsedContents.data(), size);
				in.close();
			}
			else
			{
				logger::error("open_gl_shader::parse_shader_contents_from_file | An error occurred.");
			}
		}
		else
		{
			logger::error("open_gl_shader::parse_shader_contents_from_file | An error occurred.");
		}
		return parsedContents;
	}

	std::map<shader_type, std::string> open_gl_shader::process_shader_sources(
		const std::string& vertex_source, const std::string& fragment_source)
	{
		std::map<shader_type, std::string> shaderSources = {};
		shaderSources[fragment] = fragment_source;
		shaderSources[vertex] = vertex_source;

		return shaderSources;
	}

	void open_gl_shader::compile_shader()
	{
		const uint32_t shaderProgram = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs{};
		int shaderIndex = 0;
		// Iterate through the shader sources.
		for (auto& shader : m_shader_sources)
		{
			const shader_type shaderType = shader.first;
			const std::string& shaderSource = shader.second;

			// Creating the program
			const GLuint open_gl_shader = glCreateShader(shaderType);
			// Assigning source
			const GLchar* sourceCStr = shaderSource.c_str();
			glShaderSource(open_gl_shader, 1, &sourceCStr, nullptr);

			// Compiling the shader
			glCompileShader(open_gl_shader);

			// Error handling
			GLint success = 0;
			glGetShaderiv(open_gl_shader, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(open_gl_shader, GL_INFO_LOG_LENGTH, &maxLength);

				// Get error log.
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(open_gl_shader, maxLength, &maxLength, infoLog.data());

				// Delete the shader.
				glDeleteShader(open_gl_shader);

				// Print error log.
				logger::error(
					"open_gl_shader::compile_shader | Shader compilation failed: " + std::string(
						infoLog.begin(), infoLog.end()));
				break;
			}

			// Attach
			glAttachShader(shaderProgram, open_gl_shader);
			glShaderIDs[shaderIndex] = open_gl_shader;
			shaderIndex++;
		}
		// Assign program ID
		m_object_handle = shaderProgram;

		// Link the program
		glLinkProgram(m_object_handle);

		// Error handling
		GLint success = 0;
		glGetProgramiv(m_object_handle, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_object_handle, GL_INFO_LOG_LENGTH, &maxLength);

			// Getting the error
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_object_handle, maxLength, &maxLength, infoLog.data());

			// Delete the program
			glDeleteProgram(m_object_handle);

			// Delete shaders.
			for (const auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// Print error log.
			logger::error(
				"open_gl_shader::compile_shader | Shader linking failed: " + std::string(infoLog.begin(), infoLog.end()));
			return;
		}

		logger::info("open_gl_shader::compile_shader | Shader Compiled Successfully!");

		// Cleanup.
		for (const auto id : glShaderIDs)
		{
			glDetachShader(m_object_handle, id);
			glDeleteShader(id);
		}
	}
}
