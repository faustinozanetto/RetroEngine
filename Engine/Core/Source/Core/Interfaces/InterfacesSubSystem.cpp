#include "pch.h"

#include "InterfacesSubSystem.h"

#include "Core/Application/RetroApplication.h"
#include "Renderer/Window/Window.h"

#include "imgui.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace Retro
{
    InterfacesSubSystem::InterfacesSubSystem() : SubSystem("InterfacesSubSystem")
    {
    }

    InterfacesSubSystem::~InterfacesSubSystem()
    {
    }

    void InterfacesSubSystem::OnSubSystemStart()
    {
        const RetroApplication& retroApp = RetroApplication::GetApplication();
        const auto window = static_cast<GLFWwindow*>(retroApp.GetWindow().GetNativeWindow());

        // ImGui initialization
        IMGUI_CHECKVERSION();
        // Create the ImGui context
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::StyleColorsLight();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void InterfacesSubSystem::OnSubSystemStop()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void InterfacesSubSystem::InitializeImGui()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void InterfacesSubSystem::TerminateImGui()
    {
        // Size
        ImGuiIO& io = ImGui::GetIO();
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    Scope<InterfacesSubSystem> InterfacesSubSystem::Create()
    {
        return CreateScope<InterfacesSubSystem>();
    }
}
