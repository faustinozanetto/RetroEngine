#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "EngineCore.h"
#include "imgui.h"
#include "Core/EntryPoint.h"
#include "Core/Interfaces/InterfaceLayer.h"
#include "Renderer/Renderables/Renderable.h"
#include "Renderer/Renderables/Model/Model.h"
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

        m_Model = Retro::Renderer::Model::Create("Assets/Models/Tom.fbx");

        auto texture = Retro::Renderer::Texture::Create({
            "Assets/Textures/cat.png",
            Retro::Renderer::TextureFiltering::Nearest,
            Retro::Renderer::TextureWrapping::Repeat,
        });
        Retro::Renderer::FMaterialTexture albedoTexture = {
            Retro::Renderer::EMaterialTextureType::Albedo, texture, true
        };
        std::map<Retro::Renderer::EMaterialTextureType, Retro::Renderer::FMaterialTexture> textures = {
            {Retro::Renderer::EMaterialTextureType::Albedo, albedoTexture},
        };
        Retro::Renderer::FMaterialSpecification materialSpecification = {
            m_Shader,
            textures,
            glm::vec4(1.0f, 0.23f, 0.5f, 1.0f)
        };
        m_Material = Retro::Renderer::Material::Create(
            materialSpecification);
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
        transform = glm::scale(transform, glm::vec3(glm::cos(Retro::Renderer::Renderer::GetTime()) * 2.0f));
       // transform = glm::rotate(transform, static_cast<float>(Retro::Renderer::Renderer::GetTime()) * 5.0f,
                                //glm::vec3(0.0f, 0.0f, 0.0f));

        Retro::Renderer::Renderer::SubmitCommand({
        m_Shader, m_Square->GetVertexArrayBuffer(), m_Material, transform});
        /*for (const auto& renderable : m_Model->GetModelRenderables())
        {
            Retro::Renderer::Renderer::SubmitCommand({
                m_Shader, renderable->GetVertexArrayBuffer(), m_Material, transform
            });
        }*/
    }

private:
    Retro::Ref<Retro::Renderer::Shader> m_Shader;
    Retro::Ref<Retro::Renderer::Renderable> m_Renderable;
    Retro::Ref<Retro::Renderer::Renderable> m_Square;
    Retro::Ref<Retro::Renderer::Material> m_Material;
    Retro::Ref<Retro::Renderer::Model> m_Model;
};

class SandboxInterfaceLayer : public Retro::InterfaceLayer
{
public:
    SandboxInterfaceLayer() : InterfaceLayer("SandboxInterfaceLayer")
    {
        m_UseVsync = Retro::RetroApplication::GetApplication().GetWindow()->IsVSyncEnabled();
    }

    void OnInterfaceRenderer() override
    {
        ImGui::Begin("Sandbox");
        const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
        ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Renderer");
        if (ImGui::Button("Normal"))
        {
            Retro::Renderer::Renderer::SetRenderMode(Retro::Renderer::ERenderMode::Normal);
        }
        if (ImGui::Button("Wireframe"))
        {
            Retro::Renderer::Renderer::SetRenderMode(Retro::Renderer::ERenderMode::Wireframe);
        }
        if (ImGui::Button("Point"))
        {
            Retro::Renderer::Renderer::SetRenderMode(Retro::Renderer::ERenderMode::Point);
        }
        if (ImGui::Checkbox("VSync", &m_UseVsync))
        {
            Retro::RetroApplication::GetApplication().GetWindow()->SetEnableVSync(m_UseVsync);
        }
            ImGui::End();
    }

    void OnLayerRegistered() override
    {
    }

    void OnLayerUnregistered() override
    {
    }

    void OnLayerUpdated() override
    {
    }

private:
    bool m_UseVsync;
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
