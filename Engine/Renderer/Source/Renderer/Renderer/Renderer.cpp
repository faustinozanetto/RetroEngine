#include "pch.h"

#include "Renderer/Renderer/Renderer.h"

#include "glad/glad.h"

namespace Retro::Renderer {
	Scope<RenderingAPI> Renderer::s_RenderingAPI  = nullptr;
	Scope<RendererContext> Renderer::s_Context = nullptr;
	std::queue<RenderCommand> Renderer::s_CommandQueue = {};

	bool Renderer::Initialize(RenderingAPIType renderingAPIType, const Window& window)
	{
		RETRO_ASSERT(window, "Renderer::Initialize | Invalid window.");
		Logger::Info("Renderer::Initialize | Initializing Renderer.");
		
		// Instantiate Renderer Context.
		s_Context = RendererContext::Create(window.GetNativeWindow());
		RETRO_ASSERT(s_Context, "Renderer::Initialize | Unable to create renderer context");
		s_Context->Initialize();
		
		// Instantiate rendering api.
		s_RenderingAPI = RenderingAPI::Create(renderingAPIType);
		RETRO_ASSERT(s_RenderingAPI, "Renderer::Initialize | Unable to create rendering api.");
		s_RenderingAPI->Initialize();
		return true;
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

	void Renderer::Begin()
	{
		
	}

	void Renderer::End()
	{
		while (!s_CommandQueue.empty())
		{
			const RenderCommand command = s_CommandQueue.front();
			ProcessRenderCommand(command);
			// Remove command from the queue.
			s_CommandQueue.pop();
		}
	}

	void Renderer::SubmitCommand(const RenderCommand& command)
	{
		s_CommandQueue.push(command);
	}

	RenderingAPIType Renderer::GetRenderingAPIType()
	{
		return s_RenderingAPI->GetRenderingAPIType();
	}

	void Renderer::ProcessRenderCommand(const RenderCommand& command)
	{
		command.shader->Bind();
		command.vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		command.shader->UnBind();
		command.vao->UnBind();

	}
}
