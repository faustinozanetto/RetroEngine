#pragma once

#include "render_pass.h"

#include "core/base.h"
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

	private:
		void update_shadow_matrices();
		void generate_shadow_ubo();
		void generate_shadow_map();
		void generate_shadow_fbo();

	private:
		int blocker_samples = 25;
		int pcf_samples = 25;
		float light_radius = 0.5f;

		glm::vec2 m_scene_size;
		glm::uvec2 m_dir_light_shadow_map_res;
		glm::mat4 m_dir_light_view_projection;
		glm::mat4 m_dir_light_view;
		glm::vec2  m_dir_shadow_frustum_planes;
		float      m_dir_shadow_frustum_size;

		glm::vec2 m_shadow_map_res;
		shared<texture> m_shadow_map;

		shadow_data m_shadow_data;

		shared<uniform_buffer> m_shadow_ubo;
		shared<frame_buffer> m_shadow_fbo;
		shared<shader> m_shadow_shader;
	};
}
