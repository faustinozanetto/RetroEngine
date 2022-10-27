#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Materials/Material.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	struct render_command
	{
		shared<shader> shader;
		shared<vertex_array_buffer> vao;
		shared<material> material;
		glm::mat4 transform;

		render_command(const shared<shader> &shader, const shared<vertex_array_buffer> &vao) : shader(shader), vao(vao),
																																													 transform(1.0f)
		{
		}

		render_command(const shared<shader> &shader, const shared<vertex_array_buffer> &vao, const shared<material> &material,
									 const glm::mat4 &transform) : shader(shader), vao(vao), material(material), transform(transform)
		{
		}
	};
}
