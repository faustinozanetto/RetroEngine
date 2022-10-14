#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include "EngineCore.h"
#include "imgui.h"
#include "Core/EntryPoint.h"
#include "Core/Interfaces/InterfaceLayer.h"
#include "glad/glad.h"
#include "Core/Input/InputKey.h"
#include "Renderer/Buffers/FBO/FrameBuffer.h"
#include "Renderer/Buffers/UBO/UniformBuffer.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Renderables/Renderable.h"
#include "Renderer/Renderables/Model/Model.h"
#include "Renderer/Renderer/RenderCommand.h"
#include "Renderer/Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Textures/Texture.h"

struct CameraData
{
	glm::mat4 u_ViewProjectionMatrix;
	glm::mat4 u_ViewMatrix;
	glm::mat4 u_ProjectionMatrix;
};

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

		m_Model = Retro::Renderer::Model::Create("Assets/Models/Patrick/patrick.fbx");

		auto texture = Retro::Renderer::Texture::Create({
			"Assets/Models/Patrick/Char_Patrick.png",
			Retro::Renderer::TextureFiltering::Linear,
			Retro::Renderer::TextureWrapping::ClampEdge,
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
			2560, 1440, {
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
			}
			});

		m_Camera = new Retro::Renderer::Camera(50.0f, 0.01f, 1000.0f);
		m_CameraUBO = Retro::Renderer::UniformBuffer::Create(sizeof(CameraData), 0);
	}

	void OnLayerRegistered() override
	{
	}

	void OnLayerUnregistered() override
	{
	}

	void OnLayerUpdated() override
	{
		// Update camera UBO.
		m_Camera->SetFocalPoint(m_CameraLocation);
		m_Camera->SetFOV(m_CameraFov);
		m_CameraData.u_ViewProjectionMatrix = m_Camera->GetViewProjection();
		m_CameraData.u_ViewMatrix = m_Camera->GetViewMatrix();
		m_CameraData.u_ProjectionMatrix = m_Camera->GetProjectionMatrix();
		m_CameraUBO->SetData(&m_CameraData, sizeof(CameraData));

		m_FBO->Bind();

		if (Retro::Input::InputManager::IsKeyPressed(Retro::Input::Key::Escape)) {
			m_CameraFov = 20.0f;
		}
		else if (Retro::Input::InputManager::IsKeyPressed(Retro::Input::Key::T)) {
			m_CameraFov = 50.0f;
		}

		Retro::Renderer::Renderer::SetRendererState(Retro::Renderer::ERendererState::DEPTH_TEST, true);
		Retro::Renderer::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Retro::Renderer::Renderer::ClearScreen();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_Translate);
		// translate it down so it's at the center of the scene
		model = glm::scale(model, m_Scale); // it's a bit too big for our scene, so scale it down

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

		Retro::Renderer::Renderer::SetRendererState(Retro::Renderer::ERendererState::DEPTH_TEST, false);
		Retro::Renderer::Renderer::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Retro::Renderer::Renderer::ClearScreen();

		ImGui::Begin("Edit");
		ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 10.0f);
		ImGui::SliderFloat3("Scale", glm::value_ptr(m_Scale), 0.04f, 5.0f);
		ImGui::SliderFloat3("Location", glm::value_ptr(m_Translate), -5.0f, 5.0f);
		ImGui::SliderFloat3("Camera Pos", glm::value_ptr(m_CameraLocation), -10.0f, 10.0f);
		ImGui::SliderFloat("Camera FOV", &m_CameraFov, 1.0f, 90.0f);
		ImGui::End();

		ImGui::Begin("Assets");
		if (ImGui::TreeNode("Shaders")) {
			for (auto& shader : Retro::RetroApplication::GetApplication().GetAssetsManager()->GetShaderAssets()) {
				ImGui::Text("UUID: %lld", shader.first->Get());
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Models")) {
			for (auto& shader : Retro::RetroApplication::GetApplication().GetAssetsManager()->GetModelAssets()) {
				ImGui::Text("UUID: %lld", shader.first->Get());
			}
			ImGui::TreePop();
		}
		ImGui::End();

		m_ScreenShader->Bind();
		Retro::Renderer::Renderer::BindTexture(m_FBO->GetColorAttachmentID(0), 0);
		Retro::Renderer::Renderer::SubmitCommand({
			m_ScreenShader, m_ScreenVAO, nullptr, model
			});
		m_ScreenShader->UnBind();
	}

private:
	float m_RotationSpeed = 1.0f;
	float m_CameraFov = 45.0f;
	glm::vec3 m_Scale = glm::vec3(1.0f);
	glm::vec3 m_Translate = glm::vec3(1.0f);
	glm::vec3 m_CameraLocation = glm::vec3(1.0f);
	CameraData m_CameraData;
	Retro::Renderer::Camera* m_Camera;
	Retro::Ref<Retro::Renderer::Shader> m_Shader;
	Retro::Ref<Retro::Renderer::Shader> m_ScreenShader;
	Retro::Ref<Retro::Renderer::VertexArrayBuffer> m_ScreenVAO;
	Retro::Ref<Retro::Renderer::Material> m_Material;
	Retro::Ref<Retro::Renderer::Model> m_Model;
	Retro::Ref<Retro::Renderer::FrameBuffer> m_FBO;
	Retro::Ref<Retro::Renderer::UniformBuffer> m_CameraUBO;
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
		ImGui::ShowDemoWindow();
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
	SandboxApplication() : RetroApplication({ "Sandbox" })
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
