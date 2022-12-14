#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/base.h"
#include "core/scene/scene.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/ubo/uniform_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/camera/camera.h"
#include "renderer/lighting/lighting_environment.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/shader/shader.h"

#include "glad/glad.h"
#include "render_passes/fxaa_pass.h"
#include "render_passes/geometry_pass.h"
#include "render_passes/global_illumination_pass.h"
#include "render_passes/shadow_map_pass.h"
#include "render_passes/ssao_pass.h"

#define NUM_CASCADES 3
#define NUM_FRUSTUM_CORNERS 8

namespace retro::renderer
{
	struct camera_data
	{
		glm::mat4 u_ViewProjectionMatrix;
		glm::mat4 u_ViewMatrix;
		glm::mat4 u_ProjectionMatrix;
		glm::vec3 u_Position;
	};

	struct point_light_data
	{
		glm::vec4 position;
		glm::vec4 color;
		float intensity;
		float radius;
	};

	struct directional_light_data
	{
		glm::vec4 direction;
		glm::vec4 color;
		float intensity;
	};

	struct scene_renderer_data
	{
		camera_data m_camera_data;
		point_light_data m_pointLight;
		directional_light_data m_directional_light;
		shared<camera> m_camera;
		shared<scene> m_scene;
		shared<uniform_buffer> m_camera_ubo;
		shared<uniform_buffer> m_lights_ubo;

		shared<shader> m_lighting_shader;
		shared<shader> m_shadow_shader;
		shared<shader> m_screen_shader;

		shared<frame_buffer> m_final_frame_buffer;

		shared<vertex_array_buffer> m_screen_vao;
		shared<lighting_environment> m_lighting_environment;

		shared<shadow_map_pass> shadow_map_pass;
		shared<geometry_pass> geometry_pass;
		shared<global_illumination_pass> global_illumination_pass;
		shared<ssao_pass> ssao_pass;
		shared<fxaa_pass> fxaa_pass;

		bool fxaa_enabled;
		bool ssao_enabled;
	};

	class scene_renderer
	{
	public:
		static void initialize(const shared<camera>& camera);

		static void begin_render();
		static void end_render();
		static void set_scene(const shared<scene>& scene);

		static shared<frame_buffer>& get_geometry_frame_buffer();
		static shared<frame_buffer>& get_depth_frame_buffer();
		static shared<frame_buffer>& get_final_frame_buffer();
		static shared<lighting_environment>& get_lighting_environment();
		static shared<camera>& get_camera();
		static scene_renderer_data& get_data();
		static uint32_t get_final_texture();

		static shared<shadow_map_pass>& get_shadow_pass();
		static shared<ssao_pass>& get_ssao_pass();
		static shared<fxaa_pass>& get_fxaa_pass();

		static void resize(uint32_t width, uint32_t height);

		static GLuint generate_random_angles_texture_3d(uint32_t size);

	private:
		/* Loads essential shaders needeed for the renderer. */
		static void load_shaders();
		/* Generates the frame buffers required for the renderer. */
		static void generate_frame_buffers();
		static void create_screen_vao();
		static void create_camera(const shared<camera>& camera);
		static void setup_lights();
		static void setup_environment();
	};
}
