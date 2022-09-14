#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	struct RenderCommand
	{
		Ref<Shader> shader;
		Ref<VertexObjectBuffer> vbo;
		Ref<VertexArrayBuffer> vao;

		RenderCommand(const Ref<Shader>& shader, const Ref<VertexObjectBuffer>& vbo, const Ref<VertexArrayBuffer>& vao) : shader(shader), vbo(vbo), vao(vao) {}
	};
}
