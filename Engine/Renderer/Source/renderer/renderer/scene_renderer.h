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
        glm::vec3 color;
        float intensity;
        float radius;
    };

    struct directional_light_data
    {
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
    };

    struct shadow_data
    {
        glm::mat4 light_view_projection;
    };
    
    struct lights_data
    {
        point_light_data pointLight;
        directional_light_data directional_light;
    };

    struct scene_renderer_data
    {
        glm::mat4 light_proj;
        glm::mat4  light_view;
        float light_near = 0.1f;
        float light_far = 1000.0f;
        float orthoSize = 10.0f;
        camera_data m_camera_data;
        lights_data m_lights_data;
        shadow_data shadow_data;
        shared<camera> m_camera;
        shared<scene> m_scene;
        shared<uniform_buffer> m_camera_ubo;
        shared<uniform_buffer> m_lights_ubo;
        shared<uniform_buffer> shadows_ubo;;
        shared<shader> m_geometry_shader;
        shared<shader> m_lighting_shader;
        shared<shader> m_shadow_shader;
        shared<shader> m_screen_shader;
        shared<frame_buffer> m_geometry_frame_buffer;
        shared<frame_buffer> m_shadow_frame_buffer;
        shared<frame_buffer> m_final_frame_buffer;
        shared<vertex_array_buffer> m_screen_vao;
        shared<lighting_environment> m_lighting_environment;
        shared<shadow_map> m_shadow_map;
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

    private:
        static void load_shaders();
        static void generate_frame_buffers();
        static void create_screen_vao();
        static void create_camera(const shared<camera>& camera);
        static void setup_lights();
        static void setup_environment();
    };
}
