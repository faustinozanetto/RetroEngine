#include "pch.h"

#include "Renderer/Renderer/Renderer.h"

namespace Retro {
	Scope<RenderingAPI> Renderer::s_RenderingAPI  = nullptr;
	RendererContext* Renderer::s_Context = nullptr;

	bool Renderer::Initialize(RenderingAPIType renderingAPIType)
	{
		// Instantiate rendering api.
		Logger::Info("Renderer::Initialize | Initializing Renderer.");
		s_RenderingAPI = RenderingAPI::Create(renderingAPIType);
		return true;
	}

	void Renderer::SetRendererContext(RendererContext* context)
	{
		s_Context = context;
	}

	void Renderer::SetClearColor(glm::vec4 color)
	{
		s_RenderingAPI->SetClearColor(color);
	}

	void Renderer::ClearScreen()
	{
		s_RenderingAPI->ClearScreen();
	}

	void Renderer::SwapBuffers()
	{
		s_Context->SwapBuffers();
	}

	void Renderer::PollInput()
	{
		s_Context->PollInput();
	}

	RenderingAPI* Renderer::GetRenderingAPI()
	{
		return s_RenderingAPI.get();
	}
}