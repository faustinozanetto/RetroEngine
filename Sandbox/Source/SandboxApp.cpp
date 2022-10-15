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
#include "Renderer/Lighting/PointLight.h"
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
	glm::vec3 u_Position;
};

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float constant;
	float linear;
	float quadratic;
};

struct LightsData {
	PointLight pointLight;
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

		m_Shader = Retro::Renderer::Shader::Create("Assets/Shaders/Geometry/Geometry.vert",
			"Assets/Shaders/Geometry/Geometry.frag");

		m_ScreenShader = Retro::Renderer::Shader::Create("Assets/Shaders/Screen/Screen.vert",
			"Assets/Shaders/Screen/Screen.frag");

		m_LightingShader = Retro::Renderer::Shader::Create("Assets/Shaders/Lighting/Lighting.vert",
			"Assets/Shaders/Lighting/Lighting.frag");

		m_LightModel = Retro::Renderer::Model::Create("Assets/Models/Cube.obj");

		m_Model = Retro::Renderer::Model::Create("Assets/Models/Patrick/patrick.fbx");

		auto texture = Retro::Renderer::Texture::Create({
			"Assets/Models/Patrick/Char_Patrick.png",
			Retro::Renderer::TextureFiltering::Linear,
			Retro::Renderer::TextureWrapping::ClampEdge,
			});
		Retro::Renderer::FMaterialTexture albedoTexture = {
			texture, true
		};
		Retro::Renderer::FMaterialTexture normalTexture = {
			nullptr, false
		};
		Retro::Renderer::FMaterialTexture roughnessTexture = {
			nullptr, false
		};
		Retro::Renderer::FMaterialTexture metallicTexture = {
			nullptr, false
		};
		std::map<Retro::Renderer::EMaterialTextureType, Retro::Renderer::FMaterialTexture> textures = {
			{Retro::Renderer::EMaterialTextureType::Albedo, albedoTexture},
			{Retro::Renderer::EMaterialTextureType::Normal, normalTexture},
			{Retro::Renderer::EMaterialTextureType::Metallic, roughnessTexture},
			{Retro::Renderer::EMaterialTextureType::Roughness, metallicTexture},
		};
		Retro::Renderer::FMaterialSpecification materialSpecification = {
			m_Shader,
			textures,
			glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
			0.3f,
			0.4f,
		};
		m_Material = Retro::Renderer::Material::Create(
			materialSpecification);

		m_FBO = Retro::Renderer::FrameBuffer::Create({
			2560, 1440, {
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
				Retro::Renderer::EFrameBufferColorAttachmentFormat::RGBA16F,
			}
		});

		m_Camera = new Retro::Renderer::Camera(50.0f, 0.01f, 1000.0f);
		m_CameraUBO = Retro::Renderer::UniformBuffer::Create(sizeof(CameraData), 0);
		m_CameraUBO->SetIBOLayout({
				{Retro::Renderer::VBOElementType::Mat4, "u_ViewProjectionMatrixPos"},
				{Retro::Renderer::VBOElementType::Mat4, "u_ViewMatrix"},
				{Retro::Renderer::VBOElementType::Mat4, "u_ProjectionMatrix"},
				{Retro::Renderer::VBOElementType::FloatVec3, "u_Position"}
		}, 0);

		m_Light = Retro::CreateRef<Retro::Renderer::PointLight>();

		m_LightsUBO = Retro::Renderer::UniformBuffer::Create(sizeof(LightsData), 1);
		m_LightsUBO->SetIBOLayout({
			{Retro::Renderer::VBOElementType::FloatVec3, "position"},
			{Retro::Renderer::VBOElementType::FloatVec3, "ambient"},
			{Retro::Renderer::VBOElementType::FloatVec3, "diffuse"},
			{Retro::Renderer::VBOElementType::Float, "constant"},
			{Retro::Renderer::VBOElementType::Float, "linear"},
			{Retro::Renderer::VBOElementType::Float, "quadratic"}
		}, 1, 1+1);
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
		m_CameraUBO->Bind();
		m_Camera->SetFocalPoint(m_CameraLocation);
		m_Camera->SetFOV(m_CameraFov);
		m_CameraData.u_ViewProjectionMatrix = m_Camera->GetViewProjection();
		m_CameraData.u_ViewMatrix = m_Camera->GetViewMatrix();
		m_CameraData.u_ProjectionMatrix = m_Camera->GetProjectionMatrix();
		m_CameraData.u_Position = m_Camera->GetPosition();
		m_CameraUBO->SetData(&m_CameraData, sizeof(CameraData));
		m_CameraUBO->UnBind();

		if (Retro::Input::InputManager::IsKeyPressed(Retro::Input::Key::Escape)) {
			m_CameraFov = 20.0f;
		}
		else if (Retro::Input::InputManager::IsKeyPressed(Retro::Input::Key::T)) {
			m_CameraFov = 50.0f;
		}

		m_FBO->Bind();
		Retro::Renderer::Renderer::SetRendererState(Retro::Renderer::ERendererState::DEPTH_TEST, true);
		Retro::Renderer::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Retro::Renderer::Renderer::ClearScreen();
		m_Shader->Bind();

		// Render light model
		{
			glm::mat4 lightMat = glm::mat4(1.0f);
			lightMat = glm::translate(lightMat, m_Light->GetPosition());
			lightMat = glm::scale(lightMat, {0.25f, 0.25f, 0.25f});
			m_Shader->SetMat4("uTransform", lightMat);
			m_Shader->SetInt("material.hasAlbedoMap", 0);
			m_Shader->SetVecFloat4("material.albedo", glm::vec4(m_Light->GetColor(), 1.0f));
			Retro::Renderer::Renderer::SubmitCommand({
	m_Shader, m_LightModel->GetModelRenderables()[0]->GetVertexArrayBuffer(), nullptr, lightMat
				});
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_Translate);
		// translate it down so it's at the center of the scene
		model = glm::scale(model, m_Scale); // it's a bit too big for our scene, so scale it down

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

		// Update lights UBO
		const uint32_t size = sizeof(PointLight);
		m_LightsUBO->Bind();
		m_LightsData.pointLight.color = m_Light->GetColor();
		m_LightsData.pointLight.linear = m_Light->GetLinear();
		m_LightsData.pointLight.constant = m_Light->GetConstant();
		m_LightsData.pointLight.quadratic = m_Light->GetQuadratic();
		m_LightsData.pointLight.position = m_Light->GetPosition();
		m_LightsUBO->SetData(&m_LightsData, sizeof(LightsData));
		m_LightsUBO->UnBind();

		m_LightingShader->Bind();
		Retro::Renderer::Renderer::BindTexture(m_FBO->GetColorAttachmentID(0), 0);
		Retro::Renderer::Renderer::BindTexture(m_FBO->GetColorAttachmentID(1), 1);
		Retro::Renderer::Renderer::BindTexture(m_FBO->GetColorAttachmentID(2), 2);
		Retro::Renderer::Renderer::SubmitCommand({
			m_LightingShader, m_ScreenVAO, nullptr, glm::mat4(1.0f)
			});
		m_LightingShader->UnBind();

		ImGui::Begin("Edit");
		ImGui::SliderFloat3("Scale", glm::value_ptr(m_Scale), 0.04f, 5.0f);
		ImGui::SliderFloat3("Location", glm::value_ptr(m_Translate), -5.0f, 5.0f);
		ImGui::SliderFloat3("Camera Pos", glm::value_ptr(m_CameraLocation), -10.0f, 10.0f);
		ImGui::SliderFloat("Camera FOV", &m_CameraFov, 1.0f, 90.0f);
		if (ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPos), -5.0f, 5.0f)) {
			m_Light->SetPosition(m_LightPos);
		}
		if (ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor))) {
			m_Light->SetColor(m_LightColor);
		}
		ImGui::End();

		ImGui::Begin("Assets");

		if (ImGui::TreeNode("List")) {
			for (auto& assetsCategory : Retro::RetroApplication::GetApplication().GetAssetsManager()->GetAssets()) {
				if (ImGui::TreeNode((void*)(intptr_t)assetsCategory.first, "Asset %s", Retro::Asset::GetAssetToString(assetsCategory.first).c_str())) {
					for (auto& asset : assetsCategory.second) {
						ImGui::Text("UUID: %lld", asset.first->Get());
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		/*
		ImGui::Begin("Viewport");
		const auto viewportMinRegin = ImGui::GetWindowContentRegionMin();
		const auto viewportMaxRegin = ImGui::GetWindowContentRegionMax();
		const auto viewportOffset = ImGui::GetWindowPos();

		// Viewport bounds
		m_ViewportBounds[0] = { viewportMinRegin.x + viewportOffset.x, viewportMinRegin.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegin.x + viewportOffset.x, viewportMaxRegin.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(m_FBO->GetColorAttachmentID(1)),
			ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });

		ImGui::End();
		*/
	}

private:
	float m_RotationSpeed = 1.0f;
	float m_CameraFov = 45.0f;
	glm::vec3 m_Scale = glm::vec3(1.0f);
	glm::vec3 m_LightPos = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 m_LightColor = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 m_Translate = glm::vec3(1.0f);
	glm::vec3 m_CameraLocation = glm::vec3(1.0f);
	glm::vec2 m_ViewportBounds[2];
	glm::vec2 m_ViewportSize = { 1920.0f, 1080.0f };
	CameraData m_CameraData;
	Retro::Renderer::Camera* m_Camera;
	LightsData m_LightsData;
	Retro::Ref<Retro::Renderer::PointLight> m_Light;
	Retro::Ref<Retro::Renderer::Shader> m_Shader;
	Retro::Ref<Retro::Renderer::Shader> m_LightingShader;
	Retro::Ref<Retro::Renderer::Shader> m_ScreenShader;
	Retro::Ref<Retro::Renderer::VertexArrayBuffer> m_ScreenVAO;
	Retro::Ref<Retro::Renderer::Material> m_Material;
	Retro::Ref<Retro::Renderer::Model> m_Model;
	Retro::Ref<Retro::Renderer::Model> m_LightModel;
	Retro::Ref<Retro::Renderer::FrameBuffer> m_FBO;
	Retro::Ref<Retro::Renderer::UniformBuffer> m_CameraUBO;
	Retro::Ref<Retro::Renderer::UniformBuffer> m_LightsUBO;
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
