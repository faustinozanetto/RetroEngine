#include "pch.h"

#include "RetroApplication.h"

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
        const Renderer::FWindowSpecification windowSpecification = Renderer::FWindowSpecification();
        m_Window = Renderer::Window::Create(windowSpecification);
        // Initialize Renderer
        Renderer::Renderer::Initialize(Renderer::RenderingAPIType::OpenGL, *m_Window.get());

        // SQUARE 
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        m_VAO = Renderer::VertexArrayBuffer::Create();
        Ref<Renderer::VertexObjectBuffer> vbo = Renderer::VertexObjectBuffer::Create(vertices, sizeof(vertices));
        const Renderer::VBOLayout vboLayout = {
            {Renderer::VBOElementType::FloatVec3, "aPos",},
            {Renderer::VBOElementType::FloatVec4, "aColor",}
        };
        vbo->SetVBOLayout(vboLayout);
        m_VAO->AddVertexObjectBuffer(vbo);
        uint32_t indices[3] = { 0, 1, 2 };
        Ref<Renderer::IndexBuffer> ibo = Renderer::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VAO->SetIndexBuffer(ibo);

        m_Shader = Renderer::Shader::Create("Assets/Shaders/Basic/Basic.vert", "Assets/Shaders/Basic/Basic.frag");
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

            Renderer::Renderer::Begin();

            Renderer::RenderCommand command = {m_Shader, m_VAO};
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
