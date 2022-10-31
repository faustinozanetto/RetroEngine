﻿#pragma once

#include "glm/glm.hpp"
#include "core/base.h"
#include "core/scene/scene.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/ubo/uniform_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/camera/camera.h"
#include "renderer/lighting/lighting_environment.h"
#include "renderer/shader/shader.h"

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
		glm::vec3 position;
		glm::vec4 color;
		float constant;
		float linear;
		float quadratic;
	};

	struct lights_data
	{
		point_light_data pointLight;
	};

	struct scene_renderer_data
	{
		camera_data m_camera_data;
		lights_data m_lights_data;
		shared<camera> m_camera;
		shared<scene> m_scene;
		shared<uniform_buffer> m_camera_ubo;
		shared<uniform_buffer> m_lights_ubo;
		shared<shader> m_geometry_shader;
		shared<shader> m_lighting_shader;
		shared<shader> m_screen_shader;
		shared<frame_buffer> m_geometry_frame_buffer;
		shared<frame_buffer> m_final_frame_buffer;
		shared<vertex_array_buffer> m_screen_vao;
		shared<lighting_environment> m_lighting_environment;
	};
	
	class scene_renderer
	{
	public:
		static void initialize(const shared<camera>& camera);

		static void begin_render();
		static void end_render();
		static void set_scene(const shared<scene>& scene);

		static shared<frame_buffer>& get_geometry_frame_buffer();
		static shared<frame_buffer>& get_final_frame_buffer();
		static shared<lighting_environment>& get_lighting_environment();
		static shared<camera>& get_camera();
		static uint32_t get_final_texture();

	private:
		static void load_shaders();
		static void generate_frame_buffers();
		static void create_screen_vao();
		static void create_camera(const shared<camera>& camera);
		static void setup_lights();
		static void setup_environment();
	};
}
