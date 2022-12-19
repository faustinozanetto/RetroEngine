#include "editor_main_interface.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "core/application/retro_application.h"
#include "panels/actor_details_panel.h"
#include "panels/assets_panel.h"
#include "panels/camera_panel.h"
#include "panels/hierarchy_panel.h"
#include "panels/renderer_panel.h"
#include "panels/render_passes_panel.h"
#include "panels/statistics_panel.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
	entt::entity editor_main_interface::s_selected_actor = entt::null;
	uint32_t editor_main_interface::s_render_target = 0;
	ImGui::FileBrowser editor_main_interface::s_file_browser;

	editor_main_interface::editor_main_interface(const std::string& interface_layer_name) : interface_layer(
		interface_layer_name)
	{
	}

	editor_main_interface::~editor_main_interface()
	{
	}

	void editor_main_interface::on_layer_registered()
	{
		auto statistics_panel = create_shared<editor::statistics_panel>();
		m_panels.emplace_back(statistics_panel);
		auto hierarchy_panel = create_shared<editor::hierarchy_panel>();
		m_panels.emplace_back(hierarchy_panel);
		auto actor_details_panel = create_shared<editor::actor_details_panel>();
		m_panels.emplace_back(actor_details_panel);
		auto camera_panel = create_shared<editor::camera_panel>();
		m_panels.emplace_back(camera_panel);
		auto renderer_panel = create_shared<editor::renderer_panel>();
		m_panels.emplace_back(renderer_panel);
		shared<assets_panel> assets_panel = create_shared<editor::assets_panel>();
		m_panels.emplace_back(assets_panel);
		auto render_passes_panel = create_shared<editor::render_passes_panel>();
		m_panels.emplace_back(render_passes_panel);

		s_render_target = renderer::scene_renderer::get_final_texture();
	}

	void editor_main_interface::on_layer_unregistered()
	{
	}

	void editor_main_interface::on_layer_updated()
	{
		const float time = renderer::renderer::get_time();
		for (const auto& panel : m_panels)
		{
			panel->on_update_panel(time);
		}
	}

	void editor_main_interface::on_interface_renderer()
	{
		setup_dockspace();
		setup_toolbar();
		render_viewport();
		for (const auto& panel : m_panels)
		{
			panel->on_render_panel();
		}
		ImGui::End();
	}

	void editor_main_interface::setup_dockspace()
	{
		bool open = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		const float toolbar_height = ImGui::GetFrameHeight();
		ImGui::SetNextWindowPos({ viewport->Pos.x, viewport->Pos.y + toolbar_height });
		ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - toolbar_height });
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &open, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockMain = ImGui::GetID("MyDockspace");
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::DockSpace(dockMain, ImVec2(0.0f, 0.0f), opt_flags);
		}
	}

	void editor_main_interface::setup_toolbar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void editor_main_interface::render_viewport()
	{
		ImGui::Begin("Viewport");
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(s_render_target),
			ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });
		ImGui::End();
	}
}