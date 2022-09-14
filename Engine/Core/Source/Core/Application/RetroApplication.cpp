#include "pch.h"

#include "RetroApplication.h"

#include <glm/ext/matrix_transform.hpp>

#include "glad/glad.h"
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

        // SQUARE 
        std::vector<Renderer::RenderableVertex> vertices = {};
        vertices.emplace_back(
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec2(0.5f, 1.0f)

        );
        vertices.emplace_back(
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec2(0.0f, 0.0f)

        );
        vertices.emplace_back(

            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec2(1.0f, 0.0f)

        );
        const std::vector<uint32_t> indices = {0, 1, 2};
        m_Renderable = Renderer::Renderable::Create(vertices, indices);
        m_Shader = Renderer::Shader::Create("Assets/Shaders/Basic/Basic.vert", "Assets/Shaders/Basic/Basic.frag");
    }

    RetroApplication::~RetroApplication() = default;

    void RetroApplication::RunApplication() const
    {
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window->GetNativeWindow())))
        {
            const auto time = static_cast<float>(glfwGetTime());
            const glm::vec4 color = {glm::sin(time), glm::cos(time), glm::sin(time) + glm::cos(time), 1.0f};
            Renderer::Renderer::ClearScreen();
            Renderer::Renderer::SetClearColor(color);

            Renderer::Renderer::Begin();

            m_Shader->Bind();
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(glm::cos(time), 0.0f, glm::sin(time)));
            m_Shader->SetMat4("uTransform", transform);

            Renderer::RenderCommand command = {m_Shader, m_Renderable->GetVertexArrayBuffer()};
            Renderer::Renderer::SubmitCommand(command);

            Renderer::Renderer::End();

            Renderer::Renderer::PollInput();
            Renderer::Renderer::SwapBuffers();
        }
    }

    const Renderer::Window& RetroApplication::GetWindow() const
    {
        return *m_Window;
    }
}
