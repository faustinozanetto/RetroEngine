#include "pch.h"

#include "components.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace retro
{
	name_component::name_component(const std::string& name)
	{
		this->name = name;
	}

	transform_component::transform_component()
	{
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
	}

	transform_component::transform_component(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	const glm::mat4& transform_component::get_transform_matrix() const
	{
		return translate(glm::mat4(1.0f), position) * toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale);
	}

	model_renderer_component::model_renderer_component(const shared<renderer::model>& model)
	{
		this->model = model;
	}
}
