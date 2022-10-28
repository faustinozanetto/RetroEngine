#pragma once

#include <glm/glm.hpp>

#include "renderer/rendereables/model/model.h"

namespace retro
{
	struct name_component
	{
		std::string name;
		name_component() = default;
		name_component(const name_component&) = default;
		name_component(const std::string& name);
	};
	struct transform_component
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

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
	};
}
