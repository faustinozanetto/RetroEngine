#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Materials/Material.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	struct RenderCommand
	{
		Shared<Shader> shader;
		Shared<VertexArrayBuffer> vao;
		Shared<Material> material;
		glm::mat4 transform;

		RenderCommand(const Shared<Shader>& shader, const Shared<VertexArrayBuffer>& vao) : shader(shader), vao(vao),
			transform(1.0f)
		{
		}

		RenderCommand(const Shared<Shader>& shader, const Shared<VertexArrayBuffer>& vao, const Shared<Material>& material,
		              const glm::mat4& transform) : shader(shader), vao(vao), material(material), transform(transform)
		{
		}
	};
}
