#include "pch.h"

#include "open_gl_interface_implementation.h"

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include "core/application/retro_application.h"

namespace retro::renderer
{
	open_gl_interface_implementation::open_gl_interface_implementation()
	{
		logger::line();
		logger::info(
			"open_gl_interface_implementation::open_gl_interface_implementation | Creating interface implementation.");
		logger::line();
	}

	open_gl_interface_implementation::~open_gl_interface_implementation()
	{
	}

	void open_gl_interface_implementation::initialize_interface()
	{
		const retro_application& retroApp = retro_application::get_application();
		const auto window = static_cast<GLFWwindow*>(retroApp.get_window()->get_native_window());
		// ImGui initialization
		IMGUI_CHECKVERSION();
		// Create the ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void open_gl_interface_implementation::shutdown_interface()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void open_gl_interface_implementation::begin_interface_render()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void open_gl_interface_implementation::end_interface_render()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}