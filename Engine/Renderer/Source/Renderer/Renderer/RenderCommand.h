#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Materials/Material.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	struct RenderCommand
	{
		Ref<Shader> shader;
		Ref<VertexArrayBuffer> vao;
		Ref<Material> material;
		glm::mat4 transform;

		RenderCommand(const Ref<Shader>& shader, const Ref<VertexArrayBuffer>& vao) : shader(shader), vao(vao),
			transform(1.0f)
		{
		}

		RenderCommand(const Ref<Shader>& shader, const Ref<VertexArrayBuffer>& vao, const Ref<Material>& material,
		              const glm::mat4& transform) : shader(shader), vao(vao), material(material), transform(transform)
		{
		}
	};
}
