#include "pch.h"

#include "scene_renderer.h"

#include "renderer.h"
#include "core/scene/components.h"
#include "core/scene/actor.h"
#include "glad/glad.h"
#include "renderer/lighting/point_light.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/lighting/directional_light.h"

namespace retro::renderer
{
    static scene_renderer_data s_scene_renderer_data;

    void scene_renderer::initialize(const shared<camera>& camera)
    {
        logger::line();
        logger::info("Initializing scene_renderer");
        load_shaders();
        generate_frame_buffers();
        create_screen_vao();
        create_camera(camera);
        setup_lights();
        setup_environment();

        auto orthoSize = 20.0f;
        auto lightNear = 0.1f;

        s_scene_renderer_data.light_proj = glm::ortho(-orthoSize,
                                                      orthoSize,
                                                      -orthoSize,
                                                      orthoSize,
                                                      lightNear,
                                                      s_scene_renderer_data.light_far);

        s_scene_renderer_data.shadows_ubo = uniform_buffer::create(sizeof(shadow_data), 3);
    }

    void scene_renderer::begin_render()
    {
        // Update camera UBO.
        s_scene_renderer_data.m_camera_ubo->bind();
        s_scene_renderer_data.m_camera_data.u_ViewProjectionMatrix = s_scene_renderer_data.m_camera->
            get_view_projection();
        s_scene_renderer_data.m_camera_data.u_ViewMatrix = s_scene_renderer_data.m_camera->get_view_matrix();
        s_scene_renderer_data.m_camera_data.u_ProjectionMatrix = s_scene_renderer_data.m_camera->
            get_projection_matrix();
        s_scene_renderer_data.m_camera_data.u_Position = s_scene_renderer_data.m_camera->calculate_position();
        s_scene_renderer_data.m_camera_ubo->set_data(&s_scene_renderer_data.m_camera_data, sizeof(camera_data));
        s_scene_renderer_data.m_camera_ubo->un_bind();

        // Update lights UBO
        s_scene_renderer_data.m_lights_ubo->bind();
        const auto lights = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
            entt::get<name_component, transform_component, light_renderer_component>);
        for (auto&& [actor, model_renderer, name, transform, light] : lights.each())
        {
            if (light.type == light_type::point)
            {
                const auto point_light = dynamic_cast<retro::renderer::point_light*>(light.light.get());
                s_scene_renderer_data.m_lights_data.pointLight.color = point_light->get_color();
                s_scene_renderer_data.m_lights_data.pointLight.linear = point_light->get_linear();
                s_scene_renderer_data.m_lights_data.pointLight.constant = point_light->get_constant();
                s_scene_renderer_data.m_lights_data.pointLight.quadratic = point_light->get_quadratic();
                s_scene_renderer_data.m_lights_data.pointLight.position = transform.position;
            }
            else if (light.type == light_type::directional)
            {
                const auto directional_light = dynamic_cast<retro::renderer::directional_light*>(light.light.get());
                s_scene_renderer_data.m_lights_data.directional_light.color = directional_light->get_color();
                s_scene_renderer_data.m_lights_data.directional_light.direction = directional_light->get_direction();
                s_scene_renderer_data.m_lights_data.directional_light.position = directional_light->get_position();

                s_scene_renderer_data.light_view = glm::lookAt(
                    -(glm::normalize(directional_light->get_direction()) * s_scene_renderer_data.light_far /
                        4.0f),
                    glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                s_scene_renderer_data.shadow_data.light_view_projection =
                    s_scene_renderer_data.light_proj * s_scene_renderer_data.light_view;
            }
        }
        s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_lights_data, sizeof(lights_data));
        s_scene_renderer_data.m_lights_ubo->un_bind();

        s_scene_renderer_data.shadows_ubo->bind();
        s_scene_renderer_data.shadows_ubo->set_data(&s_scene_renderer_data.shadow_data, sizeof(shadow_data));
        s_scene_renderer_data.shadows_ubo->un_bind();

        s_scene_renderer_data.m_lighting_environment->set_view_projection(
            s_scene_renderer_data.m_camera_data.u_ViewMatrix, s_scene_renderer_data.m_camera_data.u_ProjectionMatrix);

        const auto view = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
            entt::get<name_component, transform_component>);

        /* ==================== SHADOW PASS ==================== */
        s_scene_renderer_data.m_shadow_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, true);
        renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
        s_scene_renderer_data.m_shadow_shader->bind();
        renderer::clear_screen();

        for (auto&& [actor, model_renderer, name, transform] : view.each())
        {
            for (const auto& renderable : model_renderer.model->get_model_renderables())
            {
                s_scene_renderer_data.m_shadow_shader->set_mat4("m_model", transform.get_transform_matrix());
                renderer::submit_command({
                    s_scene_renderer_data.m_shadow_shader,
                    renderable->get_vertex_array_buffer(),
                    nullptr,
                });
            }
        }
        s_scene_renderer_data.m_shadow_shader->un_bind();
        s_scene_renderer_data.m_shadow_frame_buffer->un_bind();

        /* ==================== GEOMETRY PASS ==================== */
        s_scene_renderer_data.m_geometry_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, true);
        renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
        s_scene_renderer_data.m_geometry_shader->bind();
        renderer::clear_screen();

        for (auto&& [actor, model_renderer, name, transform] : view.each())
        {
            shared<material> mat = nullptr;
            if (s_scene_renderer_data.m_scene->get_actor_registry().has<material_component>(actor))
            {
                const auto& material = s_scene_renderer_data.m_scene->get_actor_registry().get<
                    material_component>(actor);
                material.material->set_shader(s_scene_renderer_data.m_geometry_shader);
                mat = material.material;
            }
            for (const auto& renderable : model_renderer.model->get_model_renderables())
            {
                s_scene_renderer_data.m_geometry_shader->set_mat4("m_model", transform.get_transform_matrix());
                if (mat == nullptr)
                {
                    s_scene_renderer_data.m_geometry_shader->set_vec_float4("material.albedo", {0.1f, 0.1f, 0.1f, 1.0f});
                    s_scene_renderer_data.m_geometry_shader->set_float("material.metallic", 0.0f);
                    s_scene_renderer_data.m_geometry_shader->set_float("material.roughness", 1.0f);
                    s_scene_renderer_data.m_geometry_shader->set_float("material.ambient_occlusion", 1.0f);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasAlbedoMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasMetallicMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasNormalMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasRoughnessMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasAmbientOcclusionMap", 0);
                }
                renderer::submit_command({
                    s_scene_renderer_data.m_geometry_shader,
                    renderable->get_vertex_array_buffer(),
                    mat,
                });
            }
        }

        s_scene_renderer_data.m_geometry_shader->un_bind();
        s_scene_renderer_data.m_geometry_frame_buffer->un_bind();
        renderer::set_renderer_state(renderer_state::depth_test, false);

        /*==================== LIGHTING PASS ==================== */
        s_scene_renderer_data.m_final_frame_buffer->bind();
        s_scene_renderer_data.m_lighting_shader->bind();
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(0), 0);
        // position
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(1), 1); // albedo
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(2), 2); // normal
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(3), 3);
        // rough - metal - ao
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_irradiance_texture(), 4);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_prefilter_texture(), 5);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_brdf_lut_texture(), 6);
        // renderer::bind_texture(s_scene_renderer_data.m_shadow_map->get_light_depth_maps(), 7); // shadow
        s_scene_renderer_data.m_lighting_shader->set_mat4("uTransform", glm::mat4(1.0f));
        renderer::submit_command({
            s_scene_renderer_data.m_lighting_shader, s_scene_renderer_data.m_screen_vao, nullptr
        });
        s_scene_renderer_data.m_lighting_shader->un_bind();

        /*==================== FINAL PASS ====================*/
        s_scene_renderer_data.m_final_frame_buffer->bind();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, s_scene_renderer_data.m_geometry_frame_buffer->get_object_handle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_scene_renderer_data.m_final_frame_buffer->get_object_handle());
        glBlitFramebuffer(0, 0, static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_width()),
                          static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_height()), 0, 0,
                          static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_width()),
                          static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_height()),
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

    void scene_renderer::set_scene(const shared<scene>& scene)
    {
        s_scene_renderer_data.m_scene = scene;
    }

    shared<frame_buffer>& scene_renderer::get_geometry_frame_buffer()
    {
        return s_scene_renderer_data.m_geometry_frame_buffer;
    }

    shared<frame_buffer>& scene_renderer::get_depth_frame_buffer()
    {
        return s_scene_renderer_data.m_shadow_frame_buffer;
    }

    shared<frame_buffer>& scene_renderer::get_final_frame_buffer()
    {
        return s_scene_renderer_data.m_final_frame_buffer;
    }

    shared<lighting_environment>& scene_renderer::get_lighting_environment()
    {
        return s_scene_renderer_data.m_lighting_environment;
    }

    shared<camera>& scene_renderer::get_camera()
    {
        return s_scene_renderer_data.m_camera;
    }

    uint32_t scene_renderer::get_final_texture()
    {
        return s_scene_renderer_data.m_final_frame_buffer->get_attachment_id(0);
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

        s_scene_renderer_data.m_shadow_shader = shader::create(
            "Assets/Shaders/Shadows/Shadows.vert",
            "Assets/Shaders/Shadows/Shadows.frag");
    }

    void scene_renderer::generate_frame_buffers()
    {
        s_scene_renderer_data.m_geometry_frame_buffer = frame_buffer::create({
            2560, 1440,
            {
                {"position", frame_buffer_attachment_format::rgba16f},
                {"albedo", frame_buffer_attachment_format::rgba16f},
                {"normal", frame_buffer_attachment_format::rgba16f},
                {"rough-meta-ao", frame_buffer_attachment_format::rgba16f}
            }
        });

        s_scene_renderer_data.m_shadow_frame_buffer = frame_buffer::create({
            2560, 1440, {}
        });

        s_scene_renderer_data.
            m_final_frame_buffer =
            frame_buffer::create({
                2560, 1440,
                {
                    {
                        "final",
                        frame_buffer_attachment_format::rgba16f
                    }
                }
            });
    }

    void scene_renderer::create_screen_vao()
    {
        float squareVertices[5 * 4] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
        };

        // Fill index buffer
        uint32_t squareIndices[6] = {
            0, 3, 1, // first triangle
            1, 3, 2, // second triangle
        };
        s_scene_renderer_data.m_screen_vao = vertex_array_buffer::create();
        const auto vbo = vertex_object_buffer::create(
            squareVertices, sizeof(squareVertices));
        const auto ibo = index_buffer::create(
            squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_scene_renderer_data.m_screen_vao->bind();
        vbo->set_layout({
            {FloatVec3, "aPos"},
            {FloatVec2, "aTexCoord"}
        });
        s_scene_renderer_data.m_screen_vao->add_vertex_buffer(vbo);
        s_scene_renderer_data.m_screen_vao->set_index_buffer(ibo);
        s_scene_renderer_data.m_screen_vao->un_bind();
    }

    void scene_renderer::create_camera(const shared<camera>& camera)
    {
        s_scene_renderer_data.m_camera = camera;
        s_scene_renderer_data.m_camera_ubo = uniform_buffer::create(sizeof(camera_data), 0);
    }

    void scene_renderer::setup_lights()
    {
        s_scene_renderer_data.m_lights_ubo = uniform_buffer::create(sizeof(lights_data), 1);
    }

    void scene_renderer::setup_environment()
    {
        const shared<texture_cubemap> sky_cubemap = texture_cubemap::create({
            "Assets/Textures/HDR/alps_field_4k.hdr",
            texture_filtering::linear,
            texture_wrapping::clamp_edge
        });
        s_scene_renderer_data.m_lighting_environment = lighting_environment::create({
            sky_cubemap, 1024, 256, 1024, 1024
        });
    }
}
