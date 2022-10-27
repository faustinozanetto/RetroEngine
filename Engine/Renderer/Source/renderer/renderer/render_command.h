#pragma once

#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/materials/material.h"

namespace retro::renderer
{
	struct render_command
	{
		shared<shader> c_shader;
		shared<vertex_array_buffer> vao;
		shared<material> c_material;
		glm::mat4 transform;

		render_command(const shared<shader> &shader, const shared<vertex_array_buffer> &vao) : c_shader(shader), vao(vao),
																																													 transform(1.0f)
		{
		}

		render_command(const shared<shader> &shader, const shared<vertex_array_buffer> &vao, const shared<material> &material,
									 const glm::mat4 &transform) : c_shader(shader), vao(vao), c_material(material), transform(transform)
		{
		}
	};
}
