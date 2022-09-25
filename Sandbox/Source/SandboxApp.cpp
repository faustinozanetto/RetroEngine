#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "EngineCore.h"
#include "imgui.h"
#include "Core/EntryPoint.h"
#include "Core/Interfaces/InterfaceLayer.h"
#include "glad/glad.h"
#include "Renderer/Buffers/FBO/FrameBuffer.h"
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
        float squareVertices[5 * 4] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left 
        };

        // Fill index buffer
        uint32_t squareIndices[6] = {
            0, 3, 1, // first triangle
            1, 3, 2, // second triangle
        };
        m_ScreenVAO = Retro::Renderer::VertexArrayBuffer::Create();
        Retro::Ref<Retro::Renderer::VertexObjectBuffer> VBO = Retro::Renderer::VertexObjectBuffer::Create(
            squareVertices, sizeof(squareVertices));
        Retro::Ref<Retro::Renderer::IndexBuffer> IBO = Retro::Renderer::IndexBuffer::Create(
            squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_ScreenVAO->Bind();
        VBO->SetVBOLayout({
            {Retro::Renderer::VBOElementType::FloatVec3, "aPos"},
            {Retro::Renderer::VBOElementType::FloatVec2, "aTexCoord"}
        });
        m_ScreenVAO->AddVertexObjectBuffer(VBO);
        m_ScreenVAO->SetIndexBuffer(IBO);
        m_ScreenVAO->UnBind();

        m_Shader = Retro::Renderer::Shader::Create("Assets/Shaders/Basic/Basic.vert",
                                                   "Assets/Shaders/Basic/Basic.frag");

        m_ScreenShader = Retro::Renderer::Shader::Create("Assets/Shaders/Screen/Screen.vert",
                                                         "Assets/Shaders/Screen/Screen.frag");

        m_Model = Retro::Renderer::Model::Create("Assets/Models/Luigi/source/Luigi.fbx");

        auto texture = Retro::Renderer::Texture::Create({
            "Assets/Models/Luigi/textures/Texture.png",
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

        m_FBO = Retro::Renderer::FrameBuffer::Create({
            1920, 1080, {Retro::Renderer::EFrameBufferAttachmentFormat::Color}
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
        m_FBO->Bind();
        glEnable(GL_DEPTH_TEST);
        Retro::Renderer::Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Retro::Renderer::Renderer::ClearScreen();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.04f)); // it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, static_cast<float>(45.0f * Retro::Renderer::Renderer::GetTime() * 0.1f),
                            glm::vec3(0.0f, 0.1f, 0.0f));

        m_Shader->Bind();
        for (const auto& renderable : m_Model->GetModelRenderables())
        {
            m_Shader->SetMat4("uTransform", model);
            Retro::Renderer::Renderer::SubmitCommand({
                m_Shader, renderable->GetVertexArrayBuffer(), m_Material, model
            });
        }
        m_Shader->UnBind();
        m_FBO->UnBind();
        glDisable(GL_DEPTH_TEST);
        Retro::Renderer::Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
        Retro::Renderer::Renderer::ClearScreen();

        m_ScreenShader->Bind();
        glBindTextureUnit(0, m_FBO->GetRendererID());
        Retro::Renderer::Renderer::SubmitCommand({
            m_ScreenShader, m_ScreenVAO, nullptr, model
        });
        m_ScreenShader->UnBind();
    }

private
:
    Retro::Ref<Retro::Renderer::Shader> m_Shader;
    Retro::Ref<Retro::Renderer::Shader> m_ScreenShader;
    Retro::Ref<Retro::Renderer::VertexArrayBuffer> m_ScreenVAO;
    Retro::Ref<Retro::Renderer::Material> m_Material;
    Retro::Ref<Retro::Renderer::Model> m_Model;
    Retro::Ref<Retro::Renderer::FrameBuffer> m_FBO;
    Retro::Ref<Retro::Renderer::FrameBuffer> m_FBO2;
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

        /*
        bool open = true;
        const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace", &open, windowFlags);

        // DockSpace
        const ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        const float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }
        style.WindowMinSize.x = minWinSizeX;

        ImGui::End();
        */
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
