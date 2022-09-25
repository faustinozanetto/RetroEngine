﻿#include "pch.h"

#include "RetroApplication.h"

#include "Core/Interfaces/InterfaceLayer.h"
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
        // Initialize Layers and SubSystems.
        m_LayersManager = LayerManager::Create("LayersManager");
        m_InterfaceLayersManager = LayerManager::Create("InterfaceLayersManager");
        m_InterfacesSubSystem = InterfacesSubSystem::Create();
        m_InterfacesSubSystem->OnSubSystemStart();
    }

    RetroApplication::~RetroApplication() = default;

    void RetroApplication::RunApplication() const
    {
        while (!Renderer::Renderer::ShouldClose())
        {
            Renderer::Renderer::Begin();
            
            // Main Render Loop.
            {
                // Update layers.
                for (auto it = m_LayersManager->GetLayerStack().begin(); it <
                     m_LayersManager->GetLayerStack().end(); ++it)
                {
                    it->get()->OnLayerUpdated();
                }
            }

            m_InterfacesSubSystem->InitializeImGui();
            // Main Interfaces Loop.
            {
                // Update interface layer.
                for (auto it = m_InterfaceLayersManager->GetLayerStack().begin(); it <
                     m_InterfaceLayersManager->GetLayerStack().end(); ++it)
                {
                    const auto interfaceLayer = dynamic_cast<InterfaceLayer*>(it->get());
                    interfaceLayer->OnLayerUpdated();
                    interfaceLayer->OnInterfaceRenderer();
                }
            }
            m_InterfacesSubSystem->TerminateImGui();
            
            Renderer::Renderer::End();
        }
    }

    const Scope<Renderer::Window>& RetroApplication::GetWindow() const
    {
        return m_Window;
    }

    const Scope<LayerManager>& RetroApplication::GetLayersManager() const
    {
        return m_LayersManager;
    }

    const Scope<LayerManager>& RetroApplication::GetInterfaceLayersManager() const
    {
        return m_InterfaceLayersManager;
    }

    RetroApplication& RetroApplication::GetApplication()
    {
        return *s_Instance;
    }
}
