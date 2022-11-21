#include "pch.h"

#include "components.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/lighting/directional_light.h"
#include "renderer/lighting/point_light.h"

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

	model_renderer_component::model_renderer_component(const std::string& model_path)
	{
		model = renderer::model::create(model_path);
	}

	material_component::material_component(const shared<renderer::material>& material)
	{
		this->material = material;
	}

	light_renderer_component::light_renderer_component()
	{
		type = light_type::point;
		light = create_shared<renderer::point_light>();
	}

	light_renderer_component::light_renderer_component(const light_renderer_component& other)
	{
		type = other.type;
		if (type == light_type::point)
		{
			const auto point_light = reinterpret_cast<renderer::point_light*>(other.light.get());
			light = create_shared<renderer::point_light>();
			light->set_color(point_light->get_color());
			point_light->set_position(point_light->get_position());
		} else if (type == light_type::directional)
		{
			const auto directional_light = reinterpret_cast<renderer::directional_light*>(other.light.get());
			light = create_shared<renderer::directional_light>();
			light->set_color(directional_light->get_color());
		}
	}

	light_renderer_component::light_renderer_component(const shared<renderer::light>& light, light_type type)
	{
		this->light = light;
		this->type = type;
	}
}
