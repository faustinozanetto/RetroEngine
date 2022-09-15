#include "pch.h"

#include "RetroApplication.h"

#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro
{
    RetroApplication* RetroApplication::s_Instance = nullptr;

    RetroApplication::RetroApplication(const FRetroApplicationSpecification& applicationSpecification)
    {
        std::filesystem::current_path("../");
        // Update variables.
        s_Instance = this;
        m_ApplicationSpecification = applicationSpecification;
        // Initialize Logger.
        Logger::Initialize();
        Logger::Line();
        // Create window.
        const Renderer::FWindowSpecification windowSpecification = Renderer::FWindowSpecification(
            "Retro Engine", 1920, 1080, false);
        m_Window = Renderer::Window::Create(windowSpecification);
        // Initialize Renderer
        Renderer::Renderer::Initialize(Renderer::RenderingAPIType::OpenGL, *m_Window.get());
        m_LayersManager = LayerManager::Create("LayersManager");
        m_InterfaceLayersManager = LayerManager::Create("InterfaceLayersManager");
    }

    RetroApplication::~RetroApplication() = default;

    void RetroApplication::RunApplication() const
    {
        while (!Renderer::Renderer::ShouldClose())
        {
            const auto time = static_cast<float>(glfwGetTime());
            const glm::vec4 color = {glm::sin(time), glm::cos(time), glm::sin(time) + glm::cos(time), 1.0f};
            Renderer::Renderer::ClearScreen();
            Renderer::Renderer::SetClearColor(color);

            Renderer::Renderer::Begin();

            // Update layers.
            for (auto it = m_LayersManager->GetLayerStack().begin(); it <
                 m_LayersManager->GetLayerStack().end(); ++it)
            {
                it->get()->OnLayerUpdated();
            }

            // Update interface layer.
            for (auto it = m_InterfaceLayersManager->GetLayerStack().begin(); it <
                 m_InterfaceLayersManager->GetLayerStack().end(); ++it)
            {
                it->get()->OnLayerUpdated();
            }

            Renderer::Renderer::End();

            Renderer::Renderer::PollInput();
            Renderer::Renderer::SwapBuffers();
        }
    }

    const Renderer::Window& RetroApplication::GetWindow() const
    {
        return *m_Window;
    }

    const Scope<LayerManager>& RetroApplication::GetLayersManager() const
    {
        return m_LayersManager;
    }

    const Scope<LayerManager>& RetroApplication::GetInterfaceLayersManager() const
    {
        return m_InterfaceLayersManager;
    }
}
