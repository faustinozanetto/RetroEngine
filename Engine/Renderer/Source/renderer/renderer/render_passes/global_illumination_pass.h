#pragma once

#include "render_pass.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_3d.h"

constexpr int VOXEL_SIZE = 128;

namespace retro::renderer {
	class global_illumination_pass final : public render_pass
	{
	public:
		global_illumination_pass();
		~global_illumination_pass() override;

		void begin_pass() override;
		void resize(uint32_t width, uint32_t height) override;

		void visualize_voxels();

		shared<frame_buffer>& get_pass_output() override;

	private:
		void load_shaders();
		void create_textures();

	private:
		shared<model> m_voxel_model;
		shared<frame_buffer> m_voxelization_fbo;
		shared<shader> m_voxelization_shader;
		shared<shader> m_voxelization_visualize_shader;
		shared<texture_3d> m_voxelization_texture;
	};
}
