#pragma once

#include <ranges>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include "imgui.h"
#include "core/application/retro_application.h"
#include "core/entry_point.h"
#include "core/input/input_manager.h"
#include "core/interfaces/interface_layer.h"
#include "core/layers/layer.h"
#include "core/scene/components.h"
#include "core/scene/scene.h"
#include "core/scene/actor.h"
#include "glad/glad.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/ubo/uniform_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/camera/camera.h"
#include "renderer/lighting/lighting_environment.h"
#include "renderer/lighting/point_light.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_cubemap.h"

struct CameraData
{
	glm::mat4 u_ViewProjectionMatrix;
	glm::mat4 u_ViewMatrix;
	glm::mat4 u_ProjectionMatrix;
	glm::vec3 u_Position;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float constant;
	float linear;
	float quadratic;
};

struct LightsData
{
	PointLight pointLight;
};

class sandbox_layer : public retro::layer
{
public:
	sandbox_layer() : layer("Sandbox Layer")
	{
		{
			float squareVertices[5 * 4] = {
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
			};

			// Fill index buffer
			uint32_t squareIndices[6] = {
				0, 3, 1, // first triangle
				1, 3, 2, // second triangle
			};
			m_ScreenVAO = retro::renderer::vertex_array_buffer::create();
			auto VBO = retro::renderer::vertex_object_buffer::create(
				squareVertices, sizeof(squareVertices));
			auto IBO = retro::renderer::index_buffer::create(
				squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
			m_ScreenVAO->bind();
			VBO->set_layout({
				{retro::renderer::layout_element_type::FloatVec3, "aPos"},
				{retro::renderer::layout_element_type::FloatVec2, "aTexCoord"}
			});
			m_ScreenVAO->add_vertex_buffer(VBO);
			m_ScreenVAO->set_index_buffer(IBO);
			m_ScreenVAO->un_bind();
		}

		{
			m_SkyboxCubemap = retro::renderer::texture_cubemap::create({
				"Assets/Textures/HDR/belfast_farmhouse_2k.hdr",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge
			});
			m_lighting_environment = retro::renderer::lighting_environment::create(m_SkyboxCubemap);
		}

		m_Model = retro::renderer::model::create("Assets/Models/Cerberus/source/Cerberus_LP.FBX.fbx");

		m_Scene = retro::scene::create("Sandbox");
		auto actor = m_Scene->create_actor();
		actor->add_component<retro::name_component>("Test Actor");
		actor->add_component<retro::transform_component>();
		actor->add_component<retro::model_renderer_component>(m_Model);

		m_Shader = retro::renderer::shader::create("Assets/Shaders/Geometry/Geometry.vert",
		                                           "Assets/Shaders/Geometry/Geometry.frag");
		m_ScreenShader = retro::renderer::shader::create("Assets/Shaders/Screen/Screen.vert",
		                                                 "Assets/Shaders/Screen/Screen.frag");
		m_LightingShader = retro::renderer::shader::create("Assets/Shaders/Lighting/Lighting.vert",
		                                                   "Assets/Shaders/Lighting/Lighting.frag");
		m_LightModel = retro::renderer::model::create("Assets/Models/Cube.obj");

		auto albedo = retro::renderer::texture::create({
			"Assets/Models/Cerberus/textures/Cerberus_A.png",
			retro::renderer::texture_filtering::linear,
			retro::renderer::texture_wrapping::clamp_edge,
		});
		retro::renderer::material_texture albedoTexture = {
			albedo, true
		};
		auto normal = retro::renderer::texture::create({
			"Assets/Models/Cerberus/textures/Cerberus_N.png",
			retro::renderer::texture_filtering::linear,
			retro::renderer::texture_wrapping::clamp_edge,
		});
		retro::renderer::material_texture normalTexture = {
			normal, true
		};
		auto roughness = retro::renderer::texture::create({
			"Assets/Models/Cerberus/textures/Cerberus_R.png",
			retro::renderer::texture_filtering::linear,
			retro::renderer::texture_wrapping::clamp_edge,
		});
		retro::renderer::material_texture roughnessTexture = {
			roughness, true
		};
		auto metallic = retro::renderer::texture::create({
			"Assets/Models/Cerberus/textures/Cerberus_M.png",
			retro::renderer::texture_filtering::linear,
			retro::renderer::texture_wrapping::clamp_edge,
		});
		retro::renderer::material_texture metallicTexture = {
			metallic, true
		};
		const std::map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures = {
			{retro::renderer::material_texture_type::albedo, albedoTexture},
			{retro::renderer::material_texture_type::normal, normalTexture},
			{retro::renderer::material_texture_type::metallic, roughnessTexture},
			{retro::renderer::material_texture_type::roughness, metallicTexture},
		};
		const retro::renderer::material_specification materialSpecification = {
			m_Shader,
			textures,
			glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
			0.3f,
			0.4f,
		};
		m_Material = retro::renderer::material::create(
			materialSpecification);


		m_geometry_frame_buffer = retro::renderer::frame_buffer::create({
			2560, 1440, {
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
			}
		});

		m_final_frame_buffer = retro::renderer::frame_buffer::create({
			2560, 1440, {
				retro::renderer::frame_buffer_color_attachment_format::rgba16f,
			}
		});

		m_Camera = new retro::renderer::camera(50.0f, 0.01f, 1000.0f);
		m_CameraUBO = retro::renderer::uniform_buffer::create(sizeof(CameraData), 0);
		m_CameraUBO->set_layout({
			                        {retro::renderer::layout_element_type::Mat4, "u_ViewProjectionMatrixPos"},
			                        {retro::renderer::layout_element_type::Mat4, "u_ViewMatrix"},
			                        {retro::renderer::layout_element_type::Mat4, "u_ProjectionMatrix"},
			                        {retro::renderer::layout_element_type::FloatVec3, "u_Position"}
		                        },
		                        0);

		m_Light = retro::create_shared<retro::renderer::point_light>();

		m_LightsUBO = retro::renderer::uniform_buffer::create(sizeof(LightsData), 1);
		m_LightsUBO->set_layout({
			                        {retro::renderer::layout_element_type::FloatVec3, "position"},
			                        {retro::renderer::layout_element_type::FloatVec3, "ambient"},
			                        {retro::renderer::layout_element_type::FloatVec3, "diffuse"},
			                        {retro::renderer::layout_element_type::Float, "constant"},
			                        {retro::renderer::layout_element_type::Float, "linear"},
			                        {retro::renderer::layout_element_type::Float, "quadratic"}
		                        },
		                        1, 1 + 1);
	}

	void on_layer_registered() override
	{
	}

	void on_layer_unregistered() override
	{
	}

	void on_layer_updated() override
	{
		// Update camera UBO.
		m_CameraUBO->bind();
		m_Camera->set_focal_point(m_CameraLocation);
		m_Camera->set_fov(m_CameraFov);
		m_CameraData.u_ViewProjectionMatrix = m_Camera->get_view_projection();
		m_CameraData.u_ViewMatrix = m_Camera->get_view_matrix();
		m_CameraData.u_ProjectionMatrix = m_Camera->get_projection_matrix();
		m_CameraData.u_Position = m_Camera->get_position();
		m_CameraUBO->set_data(&m_CameraData, sizeof(CameraData));
		m_CameraUBO->un_bind();

		// Update lights UBO
		m_LightsUBO->bind();
		m_LightsData.pointLight.color = m_Light->get_color();
		m_LightsData.pointLight.linear = m_Light->get_linear();
		m_LightsData.pointLight.constant = m_Light->get_constant();
		m_LightsData.pointLight.quadratic = m_Light->get_quadratic();
		m_LightsData.pointLight.position = m_Light->get_position();
		m_LightsUBO->set_data(&m_LightsData, sizeof(LightsData));
		m_LightsUBO->un_bind();

		m_lighting_environment->set_view_projection(m_CameraData.u_ViewMatrix, m_CameraData.u_ProjectionMatrix);

		if (retro::input::input_manager::is_key_pressed(retro::input::key::Escape))
		{
			m_CameraFov = 20.0f;
		}
		else if (retro::input::input_manager::is_key_pressed(retro::input::key::T))
		{
			m_CameraFov = 50.0f;
		}

		m_geometry_frame_buffer->bind();
		retro::renderer::renderer::set_renderer_state(retro::renderer::renderer_state::depth_test, true);
		retro::renderer::renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
		m_Shader->bind();
		retro::renderer::renderer::clear_screen();

		// Render light model
		{
			auto lightMat = glm::mat4(1.0f);
			lightMat = translate(lightMat, m_Light->get_position());
			lightMat = scale(lightMat, {0.25f, 0.25f, 0.25f});
			m_Shader->set_mat4("uTransform", lightMat);
			m_Shader->set_int("material.hasAlbedoMap", 0);
			m_Shader->set_vec_float4("material.albedo", glm::vec4(m_Light->get_color(), 1.0f));
			retro::renderer::renderer::submit_command({
				m_Shader, m_LightModel->get_model_renderables()[0]->get_vertex_array_buffer(), nullptr, lightMat
			});
		}

		auto model = glm::mat4(1.0f);
		model = translate(model, m_Translate);
		model = rotate(model, 1.0f, m_Rotation);
		// translate it down so it's at the center of the scene
		model = scale(model, m_Scale); // it's a bit too big for our scene, so scale it down

		for (const auto& renderable : m_Model->get_model_renderables())
		{
			m_Shader->set_mat4("uTransform", model);
			retro::renderer::renderer::submit_command({
				m_Shader, renderable->get_vertex_array_buffer(), m_Material, model
			});
		}
		
		m_Shader->un_bind();
		m_geometry_frame_buffer->un_bind();

		retro::renderer::renderer::set_renderer_state(retro::renderer::renderer_state::depth_test, false);

		m_final_frame_buffer->bind();
		m_LightingShader->bind();
		retro::renderer::renderer::bind_texture(m_geometry_frame_buffer->get_color_attachment_id(0), 0);
		retro::renderer::renderer::bind_texture(m_geometry_frame_buffer->get_color_attachment_id(1), 1);
		retro::renderer::renderer::bind_texture(m_geometry_frame_buffer->get_color_attachment_id(2), 2);
		retro::renderer::renderer::bind_texture(m_geometry_frame_buffer->get_color_attachment_id(3), 3);
		retro::renderer::renderer::bind_texture(m_geometry_frame_buffer->get_color_attachment_id(4), 4);
		retro::renderer::renderer::bind_texture(m_lighting_environment->get_irradiance_texture(), 5);
		retro::renderer::renderer::bind_texture(m_lighting_environment->get_prefilter_texture(), 6);
		retro::renderer::renderer::bind_texture(m_lighting_environment->get_brdf_lut_texture(), 7);
		retro::renderer::renderer::submit_command({m_LightingShader, m_ScreenVAO, nullptr, glm::mat4(1.0f)});
		m_LightingShader->un_bind();

		m_final_frame_buffer->bind();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_geometry_frame_buffer->get_object_handle());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_final_frame_buffer->get_object_handle());
		glBlitFramebuffer(0, 0, m_final_frame_buffer->get_width(), m_final_frame_buffer->get_height(), 0, 0, m_final_frame_buffer->get_width(), m_final_frame_buffer->get_height(),
		                  GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		m_lighting_environment->render_environment();
		glDepthFunc(GL_LESS);

		m_final_frame_buffer->un_bind();

		ImGui::Begin("Edit");
		ImGui::SliderFloat3("Scale", value_ptr(m_Scale), 0.02f, 5.0f);
		ImGui::SliderFloat3("Location", value_ptr(m_Translate), -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation", value_ptr(m_Rotation), -5.0f, 5.0f);
		ImGui::SliderFloat3("Camera Pos", value_ptr(m_CameraLocation), -10.0f, 10.0f);
		ImGui::SliderFloat("Camera FOV", &m_CameraFov, 1.0f, 90.0f);
		if (ImGui::SliderFloat3("Light Position", value_ptr(m_LightPos), -5.0f, 5.0f))
		{
			m_Light->set_position(m_LightPos);
		}
		if (ImGui::ColorEdit3("Light Color", value_ptr(m_LightColor)))
		{
			m_Light->set_color(m_LightColor);
		}
		ImGui::End();

		ImGui::Begin("Assets");

		if (ImGui::TreeNode("List"))
		{
			for (auto& assetsCategory : retro::retro_application::get_application().get_assets_manager()->get_assets())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(assetsCategory.first)), "Asset %s",
				                    retro::asset::get_asset_to_string(assetsCategory.first).c_str()))
				{
					for (const auto& key : assetsCategory.second | std::views::keys)
					{
						ImGui::Text("UUID: %lld", key->get());
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::Begin("Actors");

		if (ImGui::TreeNode("List"))
		{
			int i = 0;
			for (auto& actor : m_Scene->get_actor_registry().view<retro::name_component>())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(i), "Actor %d", i))
				{
					ImGui::Text(m_Scene->get_actor_registry().get<retro::name_component>(actor).name.c_str());
					ImGui::TreePop();
				}
				i++;
			}
			ImGui::TreePop();
		}
		ImGui::End();

		
		ImGui::Begin("Viewport");
		const auto viewportMinRegin = ImGui::GetWindowContentRegionMin();
		const auto viewportMaxRegin = ImGui::GetWindowContentRegionMax();
		const auto viewportOffset = ImGui::GetWindowPos();

		// Viewport bounds
		m_ViewportBounds[0] = { viewportMinRegin.x + viewportOffset.x, viewportMinRegin.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegin.x + viewportOffset.x, viewportMaxRegin.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(m_final_frame_buffer->get_color_attachment_id(0)),
			ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });

		ImGui::End();
		
	}

private:
	float m_RotationSpeed = 1.0f;
	float m_CameraFov = 45.0f;
	glm::vec3 m_Scale = glm::vec3(1.0f);
	glm::vec3 m_LightPos = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 m_LightColor = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 m_Rotation = glm::vec3(1.0f);
	glm::vec3 m_Translate = glm::vec3(1.0f);
	glm::vec3 m_CameraLocation = glm::vec3(1.0f);
	glm::vec2 m_ViewportBounds[2];
	glm::vec2 m_ViewportSize = {1920.0f, 1080.0f};
	CameraData m_CameraData;
	retro::renderer::camera* m_Camera;
	LightsData m_LightsData;
	retro::shared<retro::scene> m_Scene;
	retro::shared<retro::renderer::texture_cubemap> m_SkyboxCubemap;
	retro::shared<retro::renderer::lighting_environment> m_lighting_environment;
	retro::shared<retro::renderer::point_light> m_Light;
	retro::shared<retro::renderer::shader> m_Shader;
	retro::shared<retro::renderer::shader> m_LightingShader;
	retro::shared<retro::renderer::shader> m_ScreenShader;
	retro::shared<retro::renderer::vertex_array_buffer> m_ScreenVAO;
	retro::shared<retro::renderer::material> m_Material;
	retro::shared<retro::renderer::model> m_Model;
	retro::shared<retro::renderer::model> m_LightModel;
	retro::shared<retro::renderer::frame_buffer> m_geometry_frame_buffer;
	retro::shared<retro::renderer::frame_buffer> m_final_frame_buffer;
	retro::shared<retro::renderer::uniform_buffer> m_CameraUBO;
	retro::shared<retro::renderer::uniform_buffer> m_LightsUBO;
};

class sandbox_interface_layer : public retro::interface_layer
{
public:
	sandbox_interface_layer() : interface_layer("SandboxInterfaceLayer")
	{
		m_use_vsync = retro::retro_application::get_application().get_window()->is_vsync_enabled();
	}

	void on_interface_renderer() override
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Sandbox");
		const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
		ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
		ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Begin("Renderer");
		if (ImGui::Button("Normal"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::normal);
		}
		if (ImGui::Button("Wireframe"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::wireframe);
		}
		if (ImGui::Button("Point"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::point);
		}
		if (ImGui::Checkbox("VSync", &m_use_vsync))
		{
			retro::retro_application::get_application().get_window()->set_vsync_enabled(m_use_vsync);
		}
		ImGui::End();
	}

	void on_layer_registered() override
	{
	}

	void on_layer_unregistered() override
	{
	}

	void on_layer_updated() override
	{
	}

private:
	bool m_use_vsync;
};

class sandbox_application : public retro::retro_application
{
public:
	sandbox_application() : retro_application({"Sandbox"})
	{
		get_layers_manager()->register_layer(retro::create_shared<sandbox_layer>());
		get_interfaces_layer_manager()->register_layer(retro::create_shared<sandbox_interface_layer>());
	}

	~sandbox_application() override
	{
	};
};

retro::retro_application* retro::create_retro_application()
{
	return new sandbox_application();
}
