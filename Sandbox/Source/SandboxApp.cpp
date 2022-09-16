#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "EngineCore.h"
#include "imgui.h"
#include "Core/EntryPoint.h"
#include "Core/Interfaces/InterfaceLayer.h"
#include "Renderer/Renderables/Renderable.h"
#include "Renderer/Renderer/RenderCommand.h"
#include "Renderer/Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Textures/Texture.h"

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

        m_Texture = Retro::Renderer::Texture::Create({
            "Assets/Textures/texture.jpeg",
            Retro::Renderer::TextureFiltering::Linear,
            Retro::Renderer::TextureWrapping::ClampBorder,
        });
    }

    void OnLayerRegistered() override
    {
    }

    void OnLayerUnregistered() override
    {
    }

    void OnLayerUpdated() override
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, abs(glm::vec3(glm::sin(Retro::Renderer::Renderer::GetTime()) * 2.0f)));
        transform = glm::rotate(transform, static_cast<float>(Retro::Renderer::Renderer::GetTime()) * 5.0f,  glm::vec3(0.0f, 0.0f, 0.1f));
        Retro::Renderer::Renderer::SubmitCommand({m_Shader, m_Square->GetVertexArrayBuffer(), m_Texture, transform});
        /*for (int i = 0; i < 10; i++)
        {
            transform = glm::translate(transform, {glm::sin(time * i), 0.0f, glm::sin(time * i)});
            Retro::Renderer::Renderer::SubmitCommand({
                m_Shader, m_Renderable->GetVertexArrayBuffer(), m_Texture, transform
            });
        }*/
    }

private:
    Retro::Ref<Retro::Renderer::Shader> m_Shader;
    Retro::Ref<Retro::Renderer::Renderable> m_Renderable;
    Retro::Ref<Retro::Renderer::Renderable> m_Square;
    Retro::Ref<Retro::Renderer::Texture> m_Texture;
};

class SandboxInterfaceLayer : public Retro::InterfaceLayer
{
public:
    SandboxInterfaceLayer() : InterfaceLayer("SandboxInterfaceLayer")
    {
    }

    void OnInterfaceRenderer() override
    {
        ImGui::Begin("Sandbox");
        const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
        ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void OnLayerRegistered() override {}
    void OnLayerUnregistered() override {}
    void OnLayerUpdated() override {}
};

class SandboxApplication : public Retro::RetroApplication
{
public:
    SandboxApplication() : RetroApplication({"Sandbox"})
    {
        GetLayersManager()->RegisterLayer(Retro::CreateRef<SandboxLayer>());
        GetInterfaceLayersManager()->RegisterLayer(Retro::CreateRef<SandboxInterfaceLayer>());
    }

    ~SandboxApplication() override
    {
    }
};

Retro::RetroApplication* Retro::CreateRetroApplication()
{
    return new SandboxApplication();
}
