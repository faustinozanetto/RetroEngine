#include "pch.h"
#include "global_illumination_pass.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "glad/glad.h"

#include "core/application/retro_application.h"
#include "core/scene/components.h"
#include "renderer/renderer/renderer.h"
#include <glm/ext/matrix_transform.hpp>

#include "renderer/renderer/scene_renderer.h"

namespace retro::renderer
{
	global_illumination_pass::global_illumination_pass()
	{
		m_voxel_model = retro_application::get_application().get_assets_manager()->create_model({ "Assets/Models/Cube.obj" });
		texture_specification vox_tex_spec = {
			glm::uvec2(1920, 1080),
			texture_filtering::linear,
			texture_wrapping::clamp_edge,
			GL_RGBA, GL_RGB16F
		};
		m_voxelization_fbo = frame_buffer::create({
			1920, 1080, false,
			{
				{"voxelization", vox_tex_spec}}
			});

		load_shaders();
		create_textures();
	}

	global_illumination_pass::~global_illumination_pass()
	{
	}

	void global_illumination_pass::begin_pass()
	{
		GLfloat clearColor[4] = { 0, 0, 0, 0 };
		GLint previousBoundTextureID;
		glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
		glBindTexture(GL_TEXTURE_3D, m_voxelization_texture->get_object_handle());
		glClearTexImage(m_voxelization_texture->get_object_handle(), 0, GL_RGBA, GL_FLOAT, &clearColor);
		glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);

		m_voxelization_shader->bind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, VOXEL_SIZE, VOXEL_SIZE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_voxelization_texture->get_object_handle());
		glUniform1i(glGetUniformLocation(m_voxelization_shader->get_object_handle(), "gVoxelizationTexture"), 0);
		glBindImageTexture(0, m_voxelization_texture->get_object_handle(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

		entt::registry& actors_registry = retro_application::get_application().get_scene_manager()->get_active_scene()->get_actor_registry();
		const auto view = actors_registry.group<model_renderer_component>(
			entt::get<name_component, transform_component>);

		for (auto&& [actor, model_renderer, name, transform] : view.each())
		{
			m_voxelization_shader->set_mat4("u_model", transform.get_transform_matrix());
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
						material->set_shader(m_voxelization_shader);
						found_material = material;
					}
					renderer::submit_command({
							m_voxelization_shader,
							renderable->get_vertex_array_buffer(),
							found_material,
						});
				}
			}
			else
			{
				// Use default material parameters when no material is found.
				material::use_default_material(m_voxelization_shader);
				for (const auto& renderable : model_renderer.model->get_model_renderables())
				{
					renderer::submit_command({
							m_voxelization_shader,
							renderable->get_vertex_array_buffer(),
							nullptr,
						});
				}
			}
		}

		glGenerateMipmap(GL_TEXTURE_3D);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void global_illumination_pass::resize(uint32_t width, uint32_t height)
	{
	}

	void global_illumination_pass::visualize_voxels()
	{
	}

	shared<frame_buffer>& global_illumination_pass::get_pass_output()
	{
		return m_voxelization_fbo;
	}

	void global_illumination_pass::load_shaders()
	{
		m_voxelization_shader = retro_application::get_application().
			get_assets_manager()->create_shader({ "Assets/Shaders/GlobalIllumination/Voxelize.vert",
				"Assets/Shaders/GlobalIllumination/Voxelize.frag","Assets/Shaders/GlobalIllumination/Voxelize.geom" });

		m_voxelization_visualize_shader = retro_application::get_application().
			get_assets_manager()->create_shader({ "Assets/Shaders/GlobalIllumination/VoxelVisualize.vert" ,
"Assets/Shaders/GlobalIllumination/VoxelVisualize.frag" });
	}

	void global_illumination_pass::create_textures()
	{
		m_voxelization_texture = retro_application::get_application().get_assets_manager()->
			create_texture_3d({ glm::uvec2(VOXEL_SIZE,VOXEL_SIZE), texture_filtering::linear_mipmap_linear,
				texture_wrapping::clamp_border, GL_RGBA8, GL_RGBA });
		m_voxelization_texture->set_filtering(texture_filtering_type::min, texture_filtering::linear_mipmap_linear);
		m_voxelization_texture->set_filtering(texture_filtering_type::mag, texture_filtering::nearest);
	}
}