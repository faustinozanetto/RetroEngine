#pragma once
#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"

namespace Retro::Renderer
{
	struct RenderCommand
	{
		Ref<VertexObjectBuffer> vbo;
		Ref<VertexArrayBuffer> vao;

		RenderCommand(const Ref<VertexObjectBuffer>& vbo, const Ref<VertexArrayBuffer>& vao) : vbo(vbo), vao(vao) {}
	};
}
