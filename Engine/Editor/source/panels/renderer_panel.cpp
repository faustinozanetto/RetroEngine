#include "renderer_panel.h"

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
	renderer_panel::renderer_panel()
	{
		m_use_vsync = retro_application::get_application().get_window()->get_window_specification().v_sync;
	}

	renderer_panel::~renderer_panel()
	{
	}

	void renderer_panel::on_render_panel()
	{
		ImGui::Begin("Renderer");
		ImGui::Separator();
		if (ImGui::CollapsingHeader("GeometryBuffer"))
		{
			for (const auto& attachment : renderer::scene_renderer::get_geometry_frame_buffer()->get_attachments_specifications())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment.first)), "Buffer %s",
					attachment.second.name.c_str()))
				{
					ImGui::Columns(2);

					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(attachment.first),
						{ 64.0f, 64.0f },
						ImVec2(0.0f, 1.0f),
						ImVec2(1.0f, 0.0f)))
					{
						editor_main_interface::s_render_target = attachment.first;
					}
					// Hover tooltip.
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Image(reinterpret_cast<ImTextureID>(attachment.first), ImVec2(256, 256),
							ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();

					ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_geometry_frame_buffer()->get_width(),
						renderer::scene_renderer::get_geometry_frame_buffer()->get_height());
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::TreeNode("FinalBuffer"))
		{
			for (const auto& attachment : renderer::scene_renderer::get_final_frame_buffer()->get_attachments_specifications())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment.first)), "Buffer %s",
					attachment.second.name.c_str()))
				{
					ImGui::Columns(2);

					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(attachment.first),
						{ 64.0f, 64.0f },
						ImVec2(0.0f, 1.0f),
						ImVec2(1.0f, 0.0f)))
					{
						editor_main_interface::s_render_target = attachment.first;
					}
					// Hover tooltip.
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Image(reinterpret_cast<ImTextureID>(attachment.first), ImVec2(256, 256),
							ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();

					ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_geometry_frame_buffer()->get_width(),
						renderer::scene_renderer::get_geometry_frame_buffer()->get_height());
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FXAA Buffer"))
		{
			for (const auto& attachment : renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_attachments_specifications())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment.first)), "Buffer %s",
					attachment.second.name.c_str()))
				{
					ImGui::Columns(2);

					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(attachment.first),
						{ 64.0f, 64.0f },
						ImVec2(0.0f, 1.0f),
						ImVec2(1.0f, 0.0f)))
					{
						editor_main_interface::s_render_target = attachment.first;
					}
					// Hover tooltip.
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Image(reinterpret_cast<ImTextureID>(attachment.first), ImVec2(256, 256),
							ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();

					ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_width(),
						renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_height());
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("SSAO Buffer"))
		{
			if (ImGui::TreeNode("SSAO Main Texture"))
			{
				ImGui::Columns(2);

				if (ImGui::ImageButton(
					reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_ssao_pass()->get_ssao()),
					{ 64.0f, 64.0f },
					ImVec2(0.0f, 1.0f),
					ImVec2(1.0f, 0.0f)))
				{
					editor_main_interface::s_render_target = renderer::scene_renderer::get_ssao_pass()->get_ssao();
				}
				// Hover tooltip.
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_ssao_pass()->get_ssao()), ImVec2(256, 256),
						ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					ImGui::EndTooltip();
				}
				ImGui::NextColumn();

				ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_width(),
					renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_height());
				ImGui::NextColumn();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("SSAO Blur Texture"))
			{
				ImGui::Columns(2);

				if (ImGui::ImageButton(
					reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_ssao_pass()->get_ssao_result()),
					{ 64.0f, 64.0f },
					ImVec2(0.0f, 1.0f),
					ImVec2(1.0f, 0.0f)))
				{
					editor_main_interface::s_render_target = renderer::scene_renderer::get_ssao_pass()->get_ssao_result();
				}
				// Hover tooltip.
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_ssao_pass()->get_ssao_result()), ImVec2(256, 256),
						ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					ImGui::EndTooltip();
				}
				ImGui::NextColumn();

				ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_width(),
					renderer::scene_renderer::get_fxaa_pass()->get_pass_output()->get_height());
				ImGui::NextColumn();
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("DepthBuffer"))
		{
			ImGui::Columns(2);

			if (ImGui::ImageButton(
				reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_depth_frame_buffer()->get_depth_attachment_id()),
				{ 64.0f, 64.0f },
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)))
			{
				editor_main_interface::s_render_target = renderer::scene_renderer::get_depth_frame_buffer()->get_depth_attachment_id();
			}
			// Hover tooltip.
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_depth_frame_buffer()->get_depth_attachment_id()), ImVec2(256, 256),
					ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				ImGui::EndTooltip();
			}
			ImGui::NextColumn();

			ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_geometry_frame_buffer()->get_width(),
				renderer::scene_renderer::get_geometry_frame_buffer()->get_height());
			ImGui::NextColumn();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Environment"))
		{
			const std::string& capture_dimensions = std::format(
				"{}x{}",
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().capture_size,
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().capture_size);
			editor_interface_utils::draw_property("Capture Dimensions", capture_dimensions);
			const std::string& irradiance_size = std::format(
				"{}x{}",
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().irradiance_size,
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().irradiance_size);
			editor_interface_utils::draw_property("Irradiance Dimensions", irradiance_size);
			const std::string& prefilter_size = std::format(
				"{}x{}",
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().prefilter_size,
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().prefilter_size);
			editor_interface_utils::draw_property("Prefilter Dimensions", prefilter_size);
			const std::string& brdf_lut_size = std::format(
				"{}x{}",
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().brdf_lut_size,
				renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().brdf_lut_size);
			editor_interface_utils::draw_property("BRDF Lut Dimensions", brdf_lut_size);
			// HDRi
			if (ImGui::TreeNode("Environment Map"))
			{
				ImGui::Image(
					reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->get_hdri_texture()),
					{ 64.0f, 64.0f },
					ImVec2(0.0f, 1.0f),
					ImVec2(1.0f, 0.0f));

				// Hover tooltip.
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->get_hdri_texture()), ImVec2(256, 256),
						ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					ImGui::EndTooltip();
				}
				ImGui::TreePop();
			}
			// BRDF Lut
			if (ImGui::TreeNode("Environment BRDF Lut"))
			{
				ImGui::Image(
					reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->get_brdf_lut_texture()),
					{ 64.0f, 64.0f },
					ImVec2(0.0f, 1.0f),
					ImVec2(1.0f, 0.0f));

				// Hover tooltip.
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->get_brdf_lut_texture()), ImVec2(256, 256),
						ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					ImGui::EndTooltip();
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		render_window_parameters();
		render_ssao_parameters();
		render_fxaa_parameters();
		ImGui::End();
	}

	void renderer_panel::render_window_parameters() const
	{
		ImGui::PushID("window_params");
		if (ImGui::CollapsingHeader("Configure Window"))
		{
			const auto& window = retro_application::get_application().get_window();
			bool vsync_enabled = window->get_window_specification().v_sync;
			if (editor_interface_utils::draw_property("Enable VSync", vsync_enabled))
			{
				window->set_vsync_enabled(vsync_enabled);
			}
		}
		ImGui::PopID();
	}

	void renderer_panel::render_ssao_parameters() const
	{
		ImGui::PushID("ssao_params");
		if (ImGui::CollapsingHeader("Configure SSAO"))
		{
			const auto& ssao_pass = renderer::scene_renderer::get_ssao_pass();
			render_pass_enabled(ssao_pass);
			editor_interface_utils::draw_property("Radius", ssao_pass->get_ssao_parameters().radius, 0.0f, 1.0f, 0.01f);
			editor_interface_utils::draw_property("Bias", ssao_pass->get_ssao_parameters().bias, 0.0f, 1.0f, 0.01f);
		}
		ImGui::PopID();
	}

	void renderer_panel::render_fxaa_parameters() const
	{
		ImGui::PushID("fxaa_params");
		if (ImGui::CollapsingHeader("Configure FXAA"))
		{
			const auto& fxaa_pass = renderer::scene_renderer::get_fxaa_pass();
			render_pass_enabled(fxaa_pass);
		}
		ImGui::PopID();
	}

	void renderer_panel::render_pass_enabled(const shared<renderer::render_pass>& render_pass) const
	{
		bool enabled = render_pass->render_pass_enabled();
		if (editor_interface_utils::draw_property("Enabled", enabled))
		{
			render_pass->set_render_pass_enabled(enabled);
		}
	}

	void renderer_panel::on_update_panel(float time)
	{
	}
}