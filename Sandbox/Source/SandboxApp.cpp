#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "EngineCore.h"
#include "Core/EntryPoint.h"
#include "GLFW/glfw3.h"
#include "Renderer/Renderables/Renderable.h"
#include "Renderer/Renderer/RenderCommand.h"
#include "Renderer/Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"

class SandboxLayer : public Retro::Layer
{
public:
    SandboxLayer() : Layer("Sandbox Layer")
    {
        // Triangle 
        std::vector<Retro::Renderer::RenderableVertex> vertices = {};
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
        m_Renderable = Retro::Renderer::Renderable::Create(vertices, indices);

        std::vector<Retro::Renderer::RenderableVertex> squareVerts = {};
        squareVerts.emplace_back(
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec2(0.0f, 0.0f)

        );
        squareVerts.emplace_back(
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec2(1.0f, 0.0f)

        );
        squareVerts.emplace_back(

            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec2(1.0f, 1.0f)

        );
        squareVerts.emplace_back(

            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec2(0.0f, 1.0f)

        );
        const std::vector<uint32_t> squareIndx = {0, 1, 2, 2, 3, 0};
        m_Square = Retro::Renderer::Renderable::Create(squareVerts, squareIndx);

        m_Shader = Retro::Renderer::Shader::Create("Assets/Shaders/Basic/Basic.vert",
                                                   "Assets/Shaders/Basic/Basic.frag");
    }

    void OnLayerRegistered() override
    {
    }

    void OnLayerUnregistered() override
    {
    }

    void OnLayerUpdated() override
    {
        const auto time = static_cast<float>(glfwGetTime());
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(0.5f));
        for (int i = 0; i < 10; i++)
        {
            transform = glm::translate(transform, {glm::sin(time * i), 0.0f, glm::sin(time * i)});
            Retro::Renderer::Renderer::SubmitCommand({m_Shader, m_Renderable->GetVertexArrayBuffer(), transform});
        }
    }

private:
    Retro::Ref<Retro::Renderer::Shader> m_Shader;
    Retro::Ref<Retro::Renderer::Renderable> m_Renderable;
    Retro::Ref<Retro::Renderer::Renderable> m_Square;
};

class SandboxApplication : public Retro::RetroApplication
{
public:
    SandboxApplication() : RetroApplication({"Sandbox"})
    {
        GetLayersManager()->RegisterLayer(Retro::CreateRef<SandboxLayer>());
    }

    ~SandboxApplication() override
    {
    }
};

Retro::RetroApplication* Retro::CreateRetroApplication()
{
    return new SandboxApplication();
}
