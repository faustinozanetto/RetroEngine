#include "pch.h"

#include "OpenGLInterfaceImplementation.h"
#include "Core/Application/RetroApplication.h"

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace Retro::Renderer
{
    OpenGLInterfaceImplementation::OpenGLInterfaceImplementation()
    {
        Logger::Line();
        Logger::Info("OpenGLInterfaceImplementation::OpenGLInterfaceImplementation | Creating interface implementation.");
        Logger::Line();
    }

    OpenGLInterfaceImplementation::~OpenGLInterfaceImplementation()
    {
    }

    void OpenGLInterfaceImplementation::InitializeInterface()
    {
        const RetroApplication &retroApp = RetroApplication::GetApplication();
        const auto window = static_cast<GLFWwindow *>(retroApp.GetWindow()->GetNativeWindow());
        // ImGui initialization
        IMGUI_CHECKVERSION();
        // Create the ImGui context
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsLight();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void OpenGLInterfaceImplementation::ShutdownInterface()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void OpenGLInterfaceImplementation::BeginInterfaceRender()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLInterfaceImplementation::EndInterfaceRender()
    {
        // Rendering
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
