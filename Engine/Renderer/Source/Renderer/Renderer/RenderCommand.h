#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	struct RenderCommand
	{
		Ref<Shader> shader;
		Ref<VertexArrayBuffer> vao;

		RenderCommand(const Ref<Shader>& shader, const Ref<VertexArrayBuffer>& vao) : shader(shader), vao(vao) {}
	};
}
