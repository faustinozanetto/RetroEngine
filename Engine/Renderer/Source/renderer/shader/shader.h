#pragma once

#include "core/base.h"
#include "core/assets/asset.h"
#include "renderer/renderer/graphics_object.h"

#include "glm/glm.hpp"

namespace retro::renderer
{
	struct shader_specification
	{
		std::string vertex_path;
		std::string fragment_path;
		std::string geometry_path;

		shader_specification(const std::string& vertex_path, const std::string& fragment_path) : vertex_path(vertex_path),
			fragment_path(fragment_path), geometry_path("") {}

		shader_specification(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path) : vertex_path(vertex_path),
			fragment_path(fragment_path), geometry_path(geometry_path) {}
	};
	class shader : public graphics_object, public asset
	{
	public:
		/* Destructor */
		~shader() override = default;

		/* Methods */
		virtual void set_float(const std::string& uniform, float value) = 0;
		virtual void set_vec_float2(const std::string& uniform, const glm::vec2& value) = 0;
		virtual void set_vec_float3(const std::string& uniform, const glm::vec3& value) = 0;
		virtual void set_vec_float4(const std::string& uniform, const glm::vec4& value) = 0;
		virtual void set_int(const std::string& uniform, int value) = 0;
		virtual void set_vec_int2(const std::string& uniform, const glm::ivec2& value) = 0;
		virtual void set_vec_int3(const std::string& uniform, const glm::ivec3& value) = 0;
		virtual void set_vec_int4(const std::string& uniform, const glm::ivec4& value) = 0;
		virtual void set_mat3(const std::string& uniform, const glm::mat3& value) = 0;
		virtual void set_mat4(const std::string& uniform, const glm::mat4& value) = 0;

		virtual int get_uniform_location(const std::string& uniform) = 0;

		/* Instantiate */
		static shared<shader> create(const shader_specification& shader_specification);
	};
}
