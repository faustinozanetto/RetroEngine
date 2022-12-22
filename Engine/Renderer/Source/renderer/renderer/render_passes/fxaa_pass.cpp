#include "pch.h"

#include "fxaa_pass.h"
#include "glad/glad.h"

#include "core/application/retro_application.h"
#include "core/assets/assets_manager.h"
#include "renderer/renderer/scene_renderer.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	fxaa_pass::fxaa_pass()
	{
		load_shaders();
		generate_vao();
		generate_frame_buffers();
	}

	fxaa_pass::~fxaa_pass()
	{
	}

	void fxaa_pass::begin_pass()
	{
		m_fxaa_frame_buffer->bind();
		m_fxaa_shader->bind();
		m_fxaa_shader->set_vec_float2("RCPFrame",
			{ static_cast<float>(1.0 / static_cast<float>(m_fxaa_frame_buffer->get_width())),
			 float(1.0 / float(m_fxaa_frame_buffer->get_height())) });
		renderer::bind_texture(scene_renderer::get_final_texture(), 0);
		renderer::submit_command({ m_fxaa_shader, m_fxaa_vao, nullptr });
		m_fxaa_shader->un_bind();
		m_fxaa_frame_buffer->un_bind();
	}

	void fxaa_pass::resize(uint32_t width, uint32_t height)
	{
		m_fxaa_frame_buffer->resize(width, height);
	}

	shared<frame_buffer>& fxaa_pass::get_pass_output()
	{
		return m_fxaa_frame_buffer;
	}

	void fxaa_pass::load_shaders()
	{
		m_fxaa_shader = assets_manager::get().create_shader({ "Assets/Shaders/FXAA/FXAA.vert",
			"Assets/Shaders/FXAA/FXAA.frag" });
	}

	void fxaa_pass::generate_vao()
	{
		float squareVertices[5 * 4] = {
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f		// top left
		};

		// Fill index buffer
		uint32_t squareIndices[6] = {
				0, 3, 1, // first triangle
				1, 3, 2, // second triangle
		};
		m_fxaa_vao = vertex_array_buffer::create();
		const auto vbo = vertex_object_buffer::create(
			squareVertices, sizeof(squareVertices));
		const auto ibo = index_buffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_fxaa_vao->bind();
		vbo->set_layout({ {FloatVec3, "aPos"},
										 {FloatVec2, "aTexCoord"} });
		m_fxaa_vao->add_vertex_buffer(vbo);
		m_fxaa_vao->set_index_buffer(ibo);
		m_fxaa_vao->un_bind();
	}

	void fxaa_pass::generate_frame_buffers()
	{
		texture_specification fxaa_tex_spec = {
			glm::uvec2(1920, 1080),
			texture_filtering::nearest,
			texture_wrapping::none,
		GL_RGBA, GL_RGB16F
		};
		m_fxaa_frame_buffer = frame_buffer::create({
			1920, 1080,false,
			{
				{"fxaa", fxaa_tex_spec}
			}
			});
	}
}