#pragma once

#include <glm/glm.hpp>

#include <cereal/types/map.hpp>
#include "cereal/types/memory.hpp"

#include "renderer/lighting/light.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"

namespace retro
{
	struct name_component
	{
		std::string name;
		name_component() = default;
		name_component(const name_component&) = default;
		name_component(const std::string& name);

		template<class Archive>
		void serialize(Archive& ar) {
			ar(name);
		}
	};

	struct transform_component
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		transform_component();
		transform_component(const transform_component&) = default;
		transform_component(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		const glm::mat4& get_transform_matrix() const;
	};

	struct model_renderer_component
	{
		shared<renderer::model> model;

		model_renderer_component() = default;
		model_renderer_component(const model_renderer_component&) = default;
		model_renderer_component(const shared<renderer::model>& model);
		model_renderer_component(const std::string& model_path);
	};

	struct material_component
	{
		std::map<int, shared<renderer::material>> materials;

		material_component() = default;
		material_component(const material_component&) = default;

		template<class Archive>
		void serialize(Archive& ar) {
			std::map<int, renderer::material> mats;
			for (auto& entry : materials)
			{
				mats.insert(std::pair(entry.first, *entry.second.get()));
			}
			ar(CEREAL_NVP(mats));
		}
	};

	enum class light_type
	{
		point = 0,
		directional = 1
	};

	struct light_renderer_component
	{
		shared<renderer::light> light;
		light_type type;

		light_renderer_component();
		light_renderer_component(const light_renderer_component& other);
		light_renderer_component(const shared<renderer::light>& light, light_type type);
	};
}
