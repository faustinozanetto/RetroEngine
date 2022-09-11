#include "pch.h"

#include "Renderer/Renderer/Renderer.h"

namespace Retro {
	Scope<RenderingAPI> Renderer::s_RenderingAPI  = nullptr;

	bool Renderer::Initialize(RenderingAPIType renderingAPIType)
	{
		// Instantiate rendering api.
		Logger::Info("Renderer::Initialize | Initializing Renderer.");
		s_RenderingAPI = RenderingAPI::Create(renderingAPIType);
		return true;
	}

	void Renderer::SetClearColor(glm::vec4 color)
	{
		s_RenderingAPI->SetClearColor(color);
	}
}