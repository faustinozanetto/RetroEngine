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
        // Update variables.
        s_Instance = this;
        m_ApplicationSpecification = applicationSpecification;
        // Initialize Logger.
        Logger::Initialize();
        Logger::Line();
        // Create window.
        const Renderer::FWindowSpecification windowSpecification = Renderer::FWindowSpecification();
        m_Window = Renderer::Window::Create(windowSpecification);
        // Initialize Renderer
        Renderer::Renderer::Initialize(Renderer::RenderingAPIType::OpenGL, *m_Window.get());
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };
        m_VBO = Renderer::VertexBuffer::Create(vertices, sizeof(vertices));
    }

    RetroApplication::~RetroApplication() = default;

    void RetroApplication::RunApplication() const
    {
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window->GetNativeWindow())))
        {
            const float time = static_cast<float>(glfwGetTime());
            const glm::vec4 color = {glm::sin(time), glm::cos(time), glm::sin(time) + glm::cos(time), 1.0f};
            Renderer::Renderer::ClearScreen();
            Renderer::Renderer::SetClearColor(color);

            Renderer::RenderCommand command = {m_VBO};
            Renderer::Renderer::SubmitCommand(command);

            Renderer::Renderer::PollInput();
            Renderer::Renderer::SwapBuffers();
        }
    }

    const Renderer::Window& RetroApplication::GetWindow() const
    {
        return *m_Window;
    }
}
