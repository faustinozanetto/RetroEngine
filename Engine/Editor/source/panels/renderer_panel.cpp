#include "renderer_panel.h"

#include "imgui.h"
#include "../editor_interface_utils.h"
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
			const std::string window_dimensions = std::format("{} x {}", window->get_window_specification().width, window->get_window_specification().height);
			editor_interface_utils::draw_property("Window Dimensions", window_dimensions);
			editor_interface_utils::draw_property("Window Title", window->get_window_specification().window_title);
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
