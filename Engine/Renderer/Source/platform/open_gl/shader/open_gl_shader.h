#pragma once

#include "map"
#include "core/assets/asset.h"
#include "glad/glad.h"
#include "renderer/shader/shader.h"

namespace retro::renderer
{
	enum shader_type
	{
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER
	};

	class open_gl_shader : public shader, public asset
	{
	public:
		/* Constructor & Destructor */
		open_gl_shader(const shader_specification& shader_specification);
		~open_gl_shader() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		void set_float(const std::string& uniform, float value) override;
		void set_vec_float2(const std::string& uniform, const glm::vec2& value) override;
		void set_vec_float3(const std::string& uniform, const glm::vec3& value) override;
		void set_vec_float4(const std::string& uniform, const glm::vec4& value) override;
		void set_int(const std::string& uniform, int value) override;
		void set_vec_int2(const std::string& uniform, const glm::ivec2& value) override;
		void set_vec_int3(const std::string& uniform, const glm::ivec3& value) override;
		void set_vec_int4(const std::string& uniform, const glm::ivec4& value) override;
		void set_mat3(const std::string& uniform, const glm::mat3& value) override;
		void set_mat4(const std::string& uniform, const glm::mat4& value) override;

		int get_uniform_location(const std::string& uniform) override;

		/* Asset */

	private:
		static std::string get_shader_type_to_string(shader_type type);
		std::string parse_shader_contents_from_file(const std::string& shader_file_path);
		std::map<shader_type, std::string> process_shader_sources(const std::string& vertex_source,
			const std::string& fragment_source);
		std::map<shader_type, std::string> process_shader_sources(const std::string& vertex_source,
			const std::string& fragment_source,
			const std::string& geometry_source);

		void compile_shader();

	private:
		std::map<shader_type, std::string> m_shader_sources;
		std::unordered_map<std::string, int> m_uniform_locations;
		int m_loaded = 0;
	};
}
