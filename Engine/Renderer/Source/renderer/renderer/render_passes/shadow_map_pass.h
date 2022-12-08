#pragma once

#include "render_pass.h"

#include "core/base.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

#include "renderer/texture/texture.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/ubo/uniform_buffer.h"
#include "renderer/shader/shader.h"

namespace retro::renderer
{
	struct shadow_data
	{
		glm::mat4 light_view_projection;
		glm::mat4 light_view;
	};

	class shadow_map_pass final : public render_pass
	{
	public:
		shadow_map_pass();
		~shadow_map_pass() override;

		void begin_pass() override;

		shared<frame_buffer>& get_pass_output() override;
		int get_blocker_samples() const { return m_blocker_samples; }
		int get_pcf_samples() const { return m_pcf_samples; }
		float get_light_radius() const { return m_light_radius; }
		float get_frustum_size() const { return m_dir_shadow_frustum_size; }
		glm::vec2& get_frustum_planes() { return m_dir_shadow_frustum_planes; }
		GLuint get_random_angles_tex() const { return m_random_angles_tex3d_id; }
		GLuint get_pcf_sampler() const { return m_pcf_sampler; }

	private:
		void update_shadow_matrices();
		void generate_shadow_ubo();
		void update_shadow_ubo();
		void generate_shadow_map();
		void generate_shadow_fbo();
		void generate_random_angles_tex();
		void generate_pcf_sampler();
		void load_shaders();

	private:
		int m_blocker_samples = 25;
		int m_pcf_samples = 25;
		float m_light_radius = 0.02f;

		glm::vec2 m_scene_size;
		glm::uvec2 m_dir_light_shadow_map_res;
		glm::mat4 m_dir_light_view_projection;
		glm::mat4 m_dir_light_view;
		glm::vec2  m_dir_shadow_frustum_planes;
		float      m_dir_shadow_frustum_size;

		glm::vec2 m_shadow_map_res;
		//shared<texture> m_shadow_map;
		GLuint m_shadow_map;
		GLuint m_random_angles_tex3d_id;
		GLuint m_pcf_sampler;

		shadow_data m_shadow_data;

		shared<uniform_buffer> m_shadow_ubo;
		shared<frame_buffer> m_shadow_fbo;
		shared<shader> m_shadow_shader;
	};
}
