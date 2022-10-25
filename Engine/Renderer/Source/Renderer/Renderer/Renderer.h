#pragma once

#include <queue>

#include "RenderCommand.h"
#include "Core/Base.h"
#include "Renderer/Renderer/RendererAPI.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Window/Window.h"
#include "glm/glm.hpp"

namespace Retro::Renderer
{
	class Renderer
	{
	public:
		/* Methods */
		static bool Initialize(RenderingAPIType renderingAPIType, const Window& window);

		static void SetClearColor(glm::vec4 color);
		static void ClearScreen();
		static void SwapBuffers();
		static void PollInput();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetRenderMode(ERenderMode renderMode);
		static void SetRendererState(ERendererState renderState, bool enabled);
		static void BindTexture(uint32_t textureHandle, uint32_t textureSlot = 0);
		static bool ShouldClose();

		static void Begin();
		static void End();

		static void SubmitCommand(const RenderCommand& command);
		static int GetRenderCommandsAmount();

		/* Getters */
		static RenderingAPIType GetRenderingAPIType();
		static double GetTime();

	private:
		static void ProcessRenderCommand(const RenderCommand& command);

		static Scope<RenderingAPI> s_RenderingAPI;
		static Scope<RendererContext> s_Context;
		static std::queue<RenderCommand> s_CommandQueue;
	};
}
