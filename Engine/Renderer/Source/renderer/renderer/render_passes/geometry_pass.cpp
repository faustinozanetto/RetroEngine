#include "pch.h"

#include "geometry_pass.h"

#include "glad/glad.h"

#include "core/application/retro_application.h"
#include "core/scene/components.h"
#include "renderer/renderer/renderer_api.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	geometry_pass::geometry_pass()
	{
		load_shaders();
		generate_geometry_fbo();
	}

	geometry_pass::~geometry_pass()
	{
	}

	void geometry_pass::begin_pass()
	{
		m_geometry_fbo->bind();
		renderer::set_renderer_state(renderer_state::depth_test, true);
		renderer::set_clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
		m_geometry_shader->bind();
		renderer::clear_screen();

		entt::registry& actors_registry = retro_application::get_application().get_scene_manager()->get_active_scene()->get_actor_registry();
		const auto view = actors_registry.group<model_renderer_component>(
			entt::get<name_component, transform_component>);

		for (auto&& [actor, model_renderer, name, transform] : view.each())
		{
			m_geometry_shader->set_mat4("m_model", transform.get_transform_matrix());
			shared<material> found_material = nullptr;
			// If the actor has a material component.
			if (actors_registry.has<material_component>(actor))
			{
				const material_component& material_comp = actors_registry.get<material_component>(actor);
				for (const auto& renderable : model_renderer.model->get_model_renderables())
				{
					if (auto it{ material_comp.materials.find(renderable->get_material_index()) }; it != std::end(material_comp.materials))
					{
						// Get the entry of the iterator.
						const auto& [material_idx, material] {*it};
						// Set shader and material.
						material->set_shader(m_geometry_shader);
						found_material = material;
					}
					renderer::submit_command({
							m_geometry_shader,
							renderable->get_vertex_array_buffer(),
							found_material,
						});
				}
			}
			else
			{
				// Use default material parameters when no material is found.
				material::use_default_material(m_geometry_shader);
				for (const auto& renderable : model_renderer.model->get_model_renderables())
				{
					renderer::submit_command({
							m_geometry_shader,
							renderable->get_vertex_array_buffer(),
							nullptr,
						});
				}
			}
		}

		m_geometry_shader->un_bind();
		m_geometry_fbo->un_bind();
	}

	shared<frame_buffer>& geometry_pass::get_pass_output()
	{
		return m_geometry_fbo;
	}

	void geometry_pass::generate_geometry_fbo()
	{
		texture_specification geom_tex_spec = {
			glm::uvec2(1920, 1080),
			texture_filtering::linear,
			texture_wrapping::clamp_edge,
			GL_RGBA, GL_RGB16F
		};
		m_geometry_fbo = frame_buffer::create({
			1920, 1080,
			{
				{"position", geom_tex_spec}, {"albedo", geom_tex_spec}, {"normal", geom_tex_spec},
				{"rough-meta-ao", geom_tex_spec}, {"viewPosition", geom_tex_spec}
			}
			});
	}

	void geometry_pass::load_shaders()
	{
		m_geometry_shader = retro_application::get_application().get_assets_manager()->create_shader({ "Assets/Shaders/Geometry/Geometry.vert",
																														 "Assets/Shaders/Geometry/Geometry.frag" });
	}
}