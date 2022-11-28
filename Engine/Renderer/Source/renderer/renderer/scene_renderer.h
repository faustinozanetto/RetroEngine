#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shadow_map.h"
#include "core/base.h"
#include "core/scene/scene.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/ubo/uniform_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/camera/camera.h"
#include "renderer/lighting/lighting_environment.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/shader/shader.h"

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

	struct csm_shadows
	{
		int m_blocker_search_samples = 25;
		int m_pcf_filter_samples = 25;
		float m_light_radius_uv = 0.5f;

		glm::vec2 scene_size;
		glm::uvec2 m_dir_light_shadow_map_res;
		glm::mat4 m_dir_light_view_projection;
		glm::mat4 m_dir_light_view;
		glm::vec2  m_dir_shadow_frustum_planes;
		float      m_dir_shadow_frustum_size;

		GLuint m_dir_shadow_map;
		GLuint m_random_angles_tex3d_id;
		GLuint m_pcf_sampler;
	};

	struct shadows_data
	{
		glm::mat4 light_view_projection;
		glm::mat4 light_view;
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
		shared<uniform_buffer> shadows_ubo;
		shared<shader> m_geometry_shader;
		shared<shader> m_lighting_shader;
		shared<shader> m_shadow_shader;
		shared<shader> m_screen_shader;
		shared<shader> m_csm_shadows_shader;
		shared<frame_buffer> m_geometry_frame_buffer;
		shared<frame_buffer> m_shadow_frame_buffer;
		shared<frame_buffer> m_final_frame_buffer;
		shared<vertex_array_buffer> m_screen_vao;
		shared<lighting_environment> m_lighting_environment;
		shared<shadow_map> m_shadow_map;
		csm_shadows m_csm_shadows;
		shadows_data shadows_data;
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

		static void create_directional_shadow_map(uint32_t width, uint32_t height);
		static void update_light_matrices();
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
