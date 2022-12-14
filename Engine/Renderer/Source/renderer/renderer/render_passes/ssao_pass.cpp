#include "pch.h"

#include "ssao_pass.h"

#include "core/application/retro_application.h"
#include "renderer/renderer/renderer.h"
#include "glad/glad.h"
#include "renderer/renderer/scene_renderer.h"

float SSAO_LERP(float a, float b, float f) { return a + f * (b - a); }

namespace retro::renderer
{
	ssao_pass::ssao_pass()
	{
		load_shaders();
		generate_vao();
		generate_frame_buffers();
		generate_sample_kernel();
		generate_noise_texture();
	}

	ssao_pass::~ssao_pass()
	{
	}

	void ssao_pass::begin_pass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		//m_ssao_frame_buffer->bind();
		m_ssao_shader->bind();
		for (int i = 0; i < m_ssao_parameters.samples_count; i++)
		{
			m_ssao_shader->set_vec_float3("u_samples[" + std::to_string(i) + "]", m_ssao_kernel[i]);
		}
		m_ssao_shader->set_int("u_kernel_size", m_ssao_kernel.size());
		m_ssao_shader->set_float("u_ssao_radius", m_ssao_parameters.radius);
		m_ssao_shader->set_float("u_ssao_bias", m_ssao_parameters.bias);
		const glm::vec2 noise_size = { retro_application::get_application().get_window()->get_window_specification().width / static_cast<float>(m_ssao_parameters.noise_texture_size),
		retro_application::get_application().get_window()->get_window_specification().height / static_cast<float>(m_ssao_parameters.noise_texture_size) };
		m_ssao_shader->set_vec_float2("u_noise_size", noise_size);
		renderer::bind_texture(scene_renderer::get_geometry_frame_buffer()->get_attachment_id(0), 0); // position
		renderer::bind_texture(scene_renderer::get_geometry_frame_buffer()->get_attachment_id(2), 1); // normal
		renderer::bind_texture(m_noise_texture->get_object_handle(), 2);
		renderer::submit_command({ m_ssao_shader, m_ssao_vao, nullptr });

		m_ssao_shader->un_bind();
		//m_ssao_frame_buffer->un_bind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		m_ssao_blur_shader->bind();
		renderer::bind_texture(ssaoColorBuffer, 0); // ssao unblurred
		renderer::submit_command({ m_ssao_blur_shader, m_ssao_vao, nullptr });
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ssao_pass::resize(uint32_t width, uint32_t height)
	{
		// Resize SSAO
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ssao_pass::load_shaders()
	{
		m_ssao_shader = retro_application::get_application().get_assets_manager()->create_shader(
			{
					"Assets/Shaders/SSAO/SSAO.vert", "Assets/Shaders/SSAO/SSAO.frag" });

		m_ssao_blur_shader = retro_application::get_application().get_assets_manager()->create_shader(
			{
					"Assets/Shaders/SSAO/SSAO.vert", "Assets/Shaders/SSAO/SSAOBlur.frag" });
	}

	void ssao_pass::generate_vao()
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
		m_ssao_vao = vertex_array_buffer::create();
		const auto vbo = vertex_object_buffer::create(
			squareVertices, sizeof(squareVertices));
		const auto ibo = index_buffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_ssao_vao->bind();
		vbo->set_layout({ {FloatVec3, "aPos"},
										 {FloatVec2, "aTexCoord"} });
		m_ssao_vao->add_vertex_buffer(vbo);
		m_ssao_vao->set_index_buffer(ibo);
		m_ssao_vao->un_bind();
	}

	void ssao_pass::generate_frame_buffers()
	{
		texture_specification ssao_tex_spec = {
			glm::uvec2(1920, 1080),
			texture_filtering::nearest,
			texture_wrapping::none,
			GL_RED, GL_RED
		};
		m_ssao_frame_buffer = frame_buffer::create({
			1920, 1080,false,
			{
				{"ssao", ssao_tex_spec}
			}
			});
		texture_specification ssao_blur_tex_spec = {
			glm::uvec2(1920, 1080),
			texture_filtering::nearest,
			texture_wrapping::none,
			GL_RED, GL_RED
		};
		m_ssao_blur_frame_buffer = frame_buffer::create({
			1920, 1080,false,
			{
				{"ssao_blur", ssao_blur_tex_spec}
			}
			});

		glGenFramebuffers(1, &ssaoFBO);
		glGenFramebuffers(1, &ssaoBlurFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

		// SSAO color buffer
		glGenTextures(1, &ssaoColorBuffer);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1920, 1080, 0, GL_RED, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "SSAO Framebuffer not complete!" << std::endl;
		// and blur stage
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glGenTextures(1, &ssaoColorBufferBlur);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1920, 1080, 0, GL_RED, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ssao_pass::generate_sample_kernel()
	{
		std::uniform_real_distribution<GLfloat> random_floats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;

		for (unsigned int i = 0; i < m_ssao_parameters.samples_count; ++i)
		{
			glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator));
			sample = glm::normalize(sample);
			sample *= random_floats(generator);
			float scale = float(i) / float(m_ssao_parameters.samples_count);

			// scale samples s.t. they're more aligned to center of kernel
			scale = SSAO_LERP(0.1f, 1.0f, scale * scale);
			sample *= scale;
			m_ssao_kernel.push_back(sample);
		}
	}

	void ssao_pass::generate_noise_texture()
	{
		delete m_noise_texture.get();
		std::uniform_real_distribution<GLfloat> random_floats(0.0, 1.0);
		std::default_random_engine generator;
		std::vector<glm::vec3> ssao_noise;
		const int loop_count = m_ssao_parameters.noise_texture_size * m_ssao_parameters.noise_texture_size;
		for (unsigned int i = 0; i < loop_count; i++)
		{
			glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssao_noise.push_back(noise);
		}

		m_noise_texture = retro_application::get_application().get_assets_manager()->create_texture_2d(
			m_ssao_parameters.noise_texture_size, m_ssao_parameters.noise_texture_size, ssao_noise.data());
		m_noise_texture->set_filtering(texture_filtering_type::min, texture_filtering::nearest);
		m_noise_texture->set_filtering(texture_filtering_type::mag, texture_filtering::nearest);
		m_noise_texture->set_wrapping(texture_wrapping_coords::s, texture_wrapping::repeat);
		m_noise_texture->set_wrapping(texture_wrapping_coords::t, texture_wrapping::repeat);
	}

	shared<frame_buffer>& ssao_pass::get_pass_output()
	{
		return m_ssao_frame_buffer;
	}
}