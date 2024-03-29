﻿#pragma once

#include <map>

#include "core/base.h"
#include "core/assets/asset.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_2d.h"

namespace retro::renderer
{
	enum class material_texture_type
	{
		albedo = 0,
		normal = 1,
		roughness = 2,
		metallic = 3,
		ambient_occlusion = 4,
	};

	struct material_texture
	{
		shared<texture_2d> mat_texture;
		bool enabled;

		material_texture() : mat_texture(nullptr), enabled(false) {}

		material_texture(const shared<texture_2d>& texture, bool is_enabled) : mat_texture(texture), enabled(is_enabled)
		{
		}
	};

	struct material_specification
	{
		shared<shader> mat_shader;
		std::string name;
		std::map<material_texture_type, material_texture> textures;
		glm::vec4 albedo = glm::vec4(1.0f);
		float metallic = 0.0f;
		float roughness = 1.0f;
		float ambient_occlusion = 0.0f;

		material_specification() = default;

		material_specification(
			const glm::vec4& albedo, float metallic, float roughness) :
			textures({}), albedo(albedo),
			metallic(metallic), roughness(roughness)
		{
		}

		material_specification(
			std::map<material_texture_type, material_texture> textures,
			const std::string& name,
			const glm::vec4& albedo, float metallic, float roughness) :
			textures(std::move(textures)), name(name), albedo(albedo),
			metallic(metallic), roughness(roughness)
		{
		}

		material_specification(
			std::map<material_texture_type, material_texture> textures,
			const std::string& name,
			const glm::vec4& albedo, float metallic, float roughness, float ao) :
			textures(std::move(textures)), name(name), albedo(albedo),
			metallic(metallic), roughness(roughness), ambient_occlusion(ao)
		{
		}

		material_specification(const shared<shader>& shader,
			const std::string& name,
			std::map<material_texture_type, material_texture> textures,
			const glm::vec4& albedo, float metallic, float roughness) : mat_shader(shader), name(name),
			textures(std::move(textures)), albedo(albedo),
			metallic(metallic), roughness(roughness)
		{
		}
	};

	class material final : public graphics_object, public asset
	{
	public:
		/* Constructor & Destructor */
		material();
		material(const material_specification& material_specification);
		~material() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		void set_shader(const shared<shader>& shader);
		void set_texture_enabled(material_texture_type type, bool enabled);

		const material_texture& get_material_texture(material_texture_type material_texture_type);
		material_specification& get_material_specification() { return m_material_specification; }

		void set_roughness(float roughness) { m_material_specification.roughness = roughness; }
		void set_metallic(float metallic) { m_material_specification.metallic = metallic; }

		static std::string get_texture_type_to_string(material_texture_type material_texture_type);
		static void use_default_material(const shared<shader>& shader);

		/* Instantiate */
		static shared<material> create();
		static shared<material> create(const material_specification& material_specification);

		template<class Archive>
		void serialize(Archive& ar) {
			ar(m_material_specification.roughness);
		}

	private:
		static std::string get_material_texture_uniform(material_texture_type material_texture_type);
		static uint32_t get_material_texture_bind_slot(material_texture_type material_texture_type);

	private:
		material_specification m_material_specification;
	};
}
