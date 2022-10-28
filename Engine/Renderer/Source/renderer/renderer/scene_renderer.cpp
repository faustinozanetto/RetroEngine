#include "pch.h"

#include "scene_renderer.h"

#include "renderer.h"
#include "core/scene/components.h"
#include "core/scene/actor.h"
#include "glad/glad.h"

namespace retro::renderer
{
    static scene_renderer_data s_scene_renderer_data;

    void scene_renderer::initialize()
    {
        logger::line();
        logger::info("Initializing scene_renderer");
        load_shaders();
        generate_frame_buffers();
        create_screen_vao();
        create_camera();
        setup_lights();
        setup_environment();
    }

    void scene_renderer::begin_render()
    {
        // Update camera UBO.
        s_scene_renderer_data.m_camera_ubo->bind();
        s_scene_renderer_data.m_camera_data.u_ViewProjectionMatrix = s_scene_renderer_data.m_camera->get_view_projection();
        s_scene_renderer_data.m_camera_data.u_ViewMatrix = s_scene_renderer_data.m_camera->get_view_matrix();
        s_scene_renderer_data.m_camera_data.u_ProjectionMatrix = s_scene_renderer_data.m_camera->get_projection_matrix();
        s_scene_renderer_data.m_camera_data.u_Position = s_scene_renderer_data.m_camera->get_position();
        s_scene_renderer_data.m_camera_ubo->set_data(&s_scene_renderer_data.m_camera_data, sizeof(camera_data));
        s_scene_renderer_data.m_camera_ubo->un_bind();

        // Update lights UBO
        /*
        s_scene_renderer_data.m_lights_ubo->bind();
        s_scene_renderer_data.m_lights_data.pointLight.color = m_Light->get_color();
        s_scene_renderer_data.m_lights_data.pointLight.linear = m_Light->get_linear();
        s_scene_renderer_data.m_lights_data.pointLight.constant = m_Light->get_constant();
        s_scene_renderer_data.m_lights_data.pointLight.quadratic = m_Light->get_quadratic();
        s_scene_renderer_data.m_lights_data.pointLight.position = m_Light->get_position();
        s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_lights_data, sizeof(lights_data));
        s_scene_renderer_data.m_lights_ubo->un_bind();
        */
        s_scene_renderer_data.m_lighting_environment->set_view_projection(
            s_scene_renderer_data.m_camera_data.u_ViewMatrix, s_scene_renderer_data.m_camera_data.u_ProjectionMatrix);

        /* ==================== GEOMETRY PASS ==================== */
        s_scene_renderer_data.m_geometry_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, true);
        renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
        s_scene_renderer_data.m_geometry_shader->bind();
        renderer::clear_screen();

        const auto view = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(entt::get<name_component, transform_component, material_component>);
        for (auto &&[actor, model_renderer, name, transform, material] : view.each())
        {
            material.material->set_shader(s_scene_renderer_data.m_geometry_shader);
            for (const auto &renderable : model_renderer.model->get_model_renderables())
            {
                s_scene_renderer_data.m_geometry_shader->set_mat4("uTransform", transform.get_transform_matrix());
                renderer::submit_command({
                    s_scene_renderer_data.m_geometry_shader,
                    renderable->get_vertex_array_buffer(),
                    material.material,
                });
            }
        }

        s_scene_renderer_data.m_geometry_shader->un_bind();
        s_scene_renderer_data.m_geometry_frame_buffer->un_bind();
        renderer::set_renderer_state(renderer_state::depth_test, false);

        /*==================== LIGHTING PASS ==================== */
        s_scene_renderer_data.m_final_frame_buffer->bind();
        s_scene_renderer_data.m_lighting_shader->bind();
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_color_attachment_id(0), 0);
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_color_attachment_id(1), 1);
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_color_attachment_id(2), 2);
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_color_attachment_id(3), 3);
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_color_attachment_id(4), 4);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_irradiance_texture(), 5);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_prefilter_texture(), 6);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_brdf_lut_texture(), 7);
        s_scene_renderer_data.m_lighting_shader->set_mat4("uTransform", glm::mat4(1.0f));
        renderer::submit_command({s_scene_renderer_data.m_lighting_shader, s_scene_renderer_data.m_screen_vao, nullptr});
        s_scene_renderer_data.m_lighting_shader->un_bind();

        /*==================== FINAL PASS ====================*/
        s_scene_renderer_data.m_final_frame_buffer->bind();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, s_scene_renderer_data.m_geometry_frame_buffer->get_object_handle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_scene_renderer_data.m_final_frame_buffer->get_object_handle());
        glBlitFramebuffer(0, 0, s_scene_renderer_data.m_final_frame_buffer->get_width(),
                          s_scene_renderer_data.m_final_frame_buffer->get_height(), 0, 0,
                          s_scene_renderer_data.m_final_frame_buffer->get_width(),
                          s_scene_renderer_data.m_final_frame_buffer->get_height(),
                          GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        s_scene_renderer_data.m_lighting_environment->render_environment();
        glDepthFunc(GL_LESS);
        s_scene_renderer_data.m_final_frame_buffer->un_bind();
    }

    void scene_renderer::end_render()
    {
    }

    void scene_renderer::set_scene(const shared<scene> &scene)
    {
        s_scene_renderer_data.m_scene = scene;
    }

    uint32_t scene_renderer::get_final_texture()
    {
        return s_scene_renderer_data.m_final_frame_buffer->get_color_attachment_id(0);
    }

    void scene_renderer::load_shaders()
    {
        s_scene_renderer_data.m_geometry_shader = shader::create("Assets/Shaders/Geometry/Geometry.vert",
                                                                 "Assets/Shaders/Geometry/Geometry.frag");
        s_scene_renderer_data.m_screen_shader = shader::create("Assets/Shaders/Screen/Screen.vert",
                                                               "Assets/Shaders/Screen/Screen.frag");
        s_scene_renderer_data.m_lighting_shader = shader::create(
            "Assets/Shaders/Lighting/Lighting.vert",
            "Assets/Shaders/Lighting/Lighting.frag");
    }

    void scene_renderer::generate_frame_buffers()
    {
        s_scene_renderer_data.m_geometry_frame_buffer = frame_buffer::create({2560, 1440, {
                                                                                              frame_buffer_color_attachment_format::rgba16f,
                                                                                              frame_buffer_color_attachment_format::rgba16f,
                                                                                              frame_buffer_color_attachment_format::rgba16f,
                                                                                              frame_buffer_color_attachment_format::rgba16f,
                                                                                              frame_buffer_color_attachment_format::rgba16f,
                                                                                          }});

        s_scene_renderer_data.m_final_frame_buffer = frame_buffer::create({2560, 1440, {
                                                                                           frame_buffer_color_attachment_format::rgba16f,
                                                                                       }});
    }

    void scene_renderer::create_screen_vao()
    {
        float squareVertices[5 * 4] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
        };

        // Fill index buffer
        uint32_t squareIndices[6] = {
            0, 3, 1, // first triangle
            1, 3, 2, // second triangle
        };
        s_scene_renderer_data.m_screen_vao = vertex_array_buffer::create();
        auto VBO = vertex_object_buffer::create(
            squareVertices, sizeof(squareVertices));
        auto IBO = index_buffer::create(
            squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_scene_renderer_data.m_screen_vao->bind();
        VBO->set_layout({{FloatVec3, "aPos"},
                         {FloatVec2, "aTexCoord"}});
        s_scene_renderer_data.m_screen_vao->add_vertex_buffer(VBO);
        s_scene_renderer_data.m_screen_vao->set_index_buffer(IBO);
        s_scene_renderer_data.m_screen_vao->un_bind();
    }

    void scene_renderer::create_camera()
    {
        s_scene_renderer_data.m_camera = create_unique<camera>(50.0f, 0.01f, 1000.0f);
        s_scene_renderer_data.m_camera_ubo = uniform_buffer::create(sizeof(camera_data), 0);
        s_scene_renderer_data.m_camera_ubo->set_layout({{Mat4, "u_ViewProjectionMatrixPos"},
                                                        {Mat4, "u_ViewMatrix"},
                                                        {Mat4, "u_ProjectionMatrix"},
                                                        {FloatVec3, "u_Position"}},
                                                       0);
    }

    void scene_renderer::setup_lights()
    {
        s_scene_renderer_data.m_lights_ubo = uniform_buffer::create(sizeof(lights_data), 1);
        s_scene_renderer_data.m_lights_ubo->set_layout({{FloatVec3, "position"},
                                                        {FloatVec3, "ambient"},
                                                        {FloatVec3, "diffuse"},
                                                        {Float, "constant"},
                                                        {Float, "linear"},
                                                        {Float, "quadratic"}},
                                                       1, 1 + 1);
    }

    void scene_renderer::setup_environment()
    {
        const shared<texture_cubemap> sky_cubemap = texture_cubemap::create({"Assets/Textures/HDR/belfast_farmhouse_2k.hdr",
                                                                             texture_filtering::linear,
                                                                             texture_wrapping::clamp_edge});
        s_scene_renderer_data.m_lighting_environment = lighting_environment::create(sky_cubemap);
    }
}
