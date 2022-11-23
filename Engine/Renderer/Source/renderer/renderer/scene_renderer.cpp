#include "pch.h"

#include "scene_renderer.h"

#include "renderer.h"
#include "core/scene/components.h"
#include "core/scene/actor.h"
#include "glad/glad.h"
#include "renderer/lighting/point_light.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
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

        s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection_matrices.resize(NUM_CASCADES);
        s_scene_renderer_data.m_csm_shadows.m_dir_light_view_matrices.resize(NUM_CASCADES);
        s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res = glm::uvec2(2048 * 4);

        create_shadow_fbo(s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.x,
                          s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.y);
        //pcf sampler
        {
            glCreateSamplers(1, &s_scene_renderer_data.m_csm_shadows.m_pcf_sampler);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_BORDER);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_BORDER);
            float color[4] = {1, 0, 0, 0};
            glSamplerParameterfv(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_BORDER_COLOR, color);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_COMPARE_MODE,
                                GL_COMPARE_REF_TO_TEXTURE);
            glSamplerParameteri(s_scene_renderer_data.m_csm_shadows.m_pcf_sampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
        uint32_t random_angles_size = 128;
        s_scene_renderer_data.m_csm_shadows.m_random_angles_tex3d_id = generate_random_angles_texture_3d(
            random_angles_size);
    }

    void scene_renderer::begin_render()
    {
        ImGui::Begin("PCSS");
        ImGui::SliderFloat("Light radius", &s_scene_renderer_data.m_csm_shadows.m_light_radius_uv, 0.0, 1.0, "%.2f");
        ImGui::Text("Average Frustum Size %f", s_scene_renderer_data.m_csm_shadows.m_avg_frustum_size);
        for (int i = 0; i < NUM_CASCADES; i++)
        {
            std::string label = "Cascade Split [" + std::to_string(i) + "]: %f";
            ImGui::Text(label.c_str(),
                        s_scene_renderer_data.m_csm_shadows.m_cascade_splits[i]);
        }
        ImGui::Text("Light Radius UV %f", s_scene_renderer_data.m_csm_shadows.m_light_radius_uv);
        ImGui::End();

        // Update camera UBO.
        s_scene_renderer_data.m_camera->update();
        s_scene_renderer_data.m_camera_data.u_ViewMatrix = s_scene_renderer_data.m_camera->get_view_matrix();
        s_scene_renderer_data.m_camera_data.u_ProjectionMatrix = s_scene_renderer_data.m_camera->
            get_projection_matrix();
        s_scene_renderer_data.m_camera_data.u_Position = s_scene_renderer_data.m_camera->get_position();
        s_scene_renderer_data.m_camera_data.u_ViewProjectionMatrix = s_scene_renderer_data.m_camera->
            get_view_projection();
        s_scene_renderer_data.m_camera_ubo->set_data(&s_scene_renderer_data.m_camera_data, sizeof(camera_data));

        // Update lights UBO
        const auto lights = s_scene_renderer_data.m_scene->get_actor_registry().group<light_renderer_component>(
            entt::get<name_component, transform_component>);
        for (auto&& [actor, light, name, transform] : lights.each())
        {
            if (light.type == light_type::point)
            {
                auto point_light = dynamic_cast<retro::renderer::point_light*>(light.light.get());
                s_scene_renderer_data.m_pointLight.position = glm::vec3(transform.position);
                s_scene_renderer_data.m_pointLight.color = glm::vec3(point_light->get_color());
                s_scene_renderer_data.m_pointLight.intensity = point_light->get_intensity();
                s_scene_renderer_data.m_pointLight.radius = point_light->get_radius();
                point_light = nullptr;
            }
            else if (light.type == light_type::directional)
            {
                auto directional_light = dynamic_cast<retro::renderer::directional_light*>(light.light.get());
                s_scene_renderer_data.m_directional_light.direction = glm::vec3(directional_light->get_direction());
                s_scene_renderer_data.m_directional_light.color = glm::vec3(directional_light->get_color());
                s_scene_renderer_data.m_directional_light.intensity = directional_light->get_intensity();
                directional_light = nullptr;
            }
        }

        s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_pointLight, sizeof(point_light_data));
        s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_directional_light,
                                                     sizeof(directional_light_data), sizeof(point_light_data));

        s_scene_renderer_data.m_lighting_environment->set_view_projection(
            s_scene_renderer_data.m_camera_data.u_ViewMatrix, s_scene_renderer_data.m_camera_data.u_ProjectionMatrix);

        const auto view = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
            entt::get<name_component, transform_component>);


        /* ==================== SHADOW PASS ==================== */
        generate_shadow_map(s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.x,
                            s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.y);
        
        s_scene_renderer_data.m_shadow_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, true);
        renderer::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});
        s_scene_renderer_data.m_shadow_shader->bind();
        renderer::clear_screen();
        
        const auto shadowview = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
            entt::get<name_component, transform_component>);

        s_scene_renderer_data.m_shadow_shader->set_float("u_light_radius_uv",
                                                         s_scene_renderer_data.m_csm_shadows.m_light_radius_uv /
                                                         s_scene_renderer_data.m_csm_shadows.m_avg_frustum_size);
        s_scene_renderer_data.m_shadow_shader->set_int("u_blocker_search_samples", 128);
        s_scene_renderer_data.m_shadow_shader->set_int("u_pcf_samples", 128);
        s_scene_renderer_data.m_shadow_shader->set_vec_float3("directionalLight.direction",
                                                              s_scene_renderer_data.m_directional_light.direction);
        for (int i = 0; i < NUM_CASCADES; i++)
        {
            s_scene_renderer_data.m_shadow_shader->set_vec_float2("u_light_frustum_planes[" + std::to_string(i) + "]",
                                                                  s_scene_renderer_data.m_csm_shadows.
                                                                  m_dir_shadow_frustum_planes[i]);
        }
        for (int i = 0; i < NUM_CASCADES; i++)
        {
            s_scene_renderer_data.m_shadow_shader->set_float("u_cascade_splits[" + std::to_string(i) + "]",
                                                             s_scene_renderer_data.m_csm_shadows.
                                                             m_cascade_splits[i]);
        }
        renderer::bind_texture(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, 8);
        renderer::bind_texture(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, 9);
        glBindSampler(9, s_scene_renderer_data.m_csm_shadows.m_pcf_sampler);
        renderer::bind_texture(s_scene_renderer_data.m_csm_shadows.m_random_angles_tex3d_id, 10);

        for (int i = 0; i < s_scene_renderer_data.m_csm_shadows.m_dir_light_view_matrices.size(); i++)
        {
            s_scene_renderer_data.m_shadow_shader->set_mat4("u_light_views[" + std::to_string(i) + "]",
                                                            s_scene_renderer_data.m_csm_shadows.
                                                            m_dir_light_view_matrices[i]);
        }
        for (int i = 0; i < s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection_matrices.size(); i++)
        {
            s_scene_renderer_data.m_shadow_shader->set_mat4("u_light_view_projections[" + std::to_string(i) + "]",
                                                            s_scene_renderer_data.m_csm_shadows.
                                                            m_dir_light_view_projection_matrices[i]);
        }

        for (auto&& [actor, model_renderer, name, transform] : shadowview.each())
        {
            s_scene_renderer_data.m_shadow_shader->set_mat4("m_model", transform.get_transform_matrix());

            for (const auto& renderable : model_renderer.model->get_model_renderables())
            {
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
        renderer::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});
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
                    s_scene_renderer_data.m_geometry_shader->
                                          set_vec_float4("material.albedo", {0.1f, 0.1f, 0.1f, 1.0f});
                    s_scene_renderer_data.m_geometry_shader->set_float("material.metallic", 0.0f);
                    s_scene_renderer_data.m_geometry_shader->set_float("material.roughness", 1.0f);
                    s_scene_renderer_data.m_geometry_shader->set_float("material.ambient_occlusion", 1.0f);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasAlbedoMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasMetallicMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasNormalMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasRoughnessMap", 0);
                    s_scene_renderer_data.m_geometry_shader->set_int("material.hasAmbientOcclusionMap", 0);


                    for (renderable_texture texture : renderable->get_textures())
                    {
                        if (texture.type == "texture_diffuse")
                        {
                            s_scene_renderer_data.m_geometry_shader->set_int("material.hasAlbedoMap", 1);
                            renderer::bind_texture(texture.id, 0);
                        }
                        else if (texture.type == "texture_normal")
                        {
                            s_scene_renderer_data.m_geometry_shader->set_int("material.hasNormalMap", 1);
                            renderer::bind_texture(texture.id, 1);
                        }
                        else if (texture.type == "texture_metallic")
                        {
                            s_scene_renderer_data.m_geometry_shader->set_int("material.hasMetallicMap", 1);
                            renderer::bind_texture(texture.id, 3);
                        }
                        else if (texture.type == "texture_roughness")
                        {
                            s_scene_renderer_data.m_geometry_shader->set_int("material.hasRoughnessMap", 1);
                            renderer::bind_texture(texture.id, 2);
                        }
                        else if (texture.type == "texture_ao")
                        {
                            s_scene_renderer_data.m_geometry_shader->set_int("material.hasAmbientOcclusionMap", 1);
                            renderer::bind_texture(texture.id, 4);
                        }
                    }
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
        for (int i = 0; i < NUM_CASCADES; i++)
        {
            s_scene_renderer_data.m_lighting_shader->set_float("u_cascade_splits[" + std::to_string(i) + "]",
                                                               s_scene_renderer_data.m_csm_shadows.
                                                               m_cascade_splits[i]);
        }
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(0), 0);
        // position
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(1), 1); // albedo
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(2), 2); // normal
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(3), 3);
        // rough - metal - ao
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_irradiance_texture(), 4);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_prefilter_texture(), 5);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_brdf_lut_texture(), 6);
        renderer::bind_texture(s_scene_renderer_data.m_shadow_frame_buffer->get_attachment_id(0), 7); // shadow
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

    GLuint scene_renderer::get_depth_frame_buffer()
    {
        return s_scene_renderer_data.m_csm_shadows.m_shadow_fbo;
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

    scene_renderer_data& scene_renderer::get_data()
    {
        return s_scene_renderer_data;
    }

    uint32_t scene_renderer::get_final_texture()
    {
        return s_scene_renderer_data.m_final_frame_buffer->get_attachment_id(0);
    }

    void scene_renderer::update_csm_splits()
    {
        float near_clip = s_scene_renderer_data.m_camera->get_near_plane();
        float far_clip = s_scene_renderer_data.m_camera->get_far_plane();
        float clip_range = far_clip - near_clip;
        float ratio = far_clip / near_clip;
        float m_cascade_split_lambda = 0.6f;

        for (uint32_t i = 0; i < NUM_CASCADES; ++i)
        {
            float p = (i + 1) / float(NUM_CASCADES);
            float log = near_clip * std::pow(ratio, p);
            float uni = near_clip + clip_range * p;
            float d = m_cascade_split_lambda * (log - uni) + uni;

            s_scene_renderer_data.m_csm_shadows.m_cascade_splits[i] = (d - near_clip) / clip_range; // to [0, 1] range
        }
    }

    void scene_renderer::update_csm_frusta()
    {
        float near_clip = s_scene_renderer_data.m_camera->get_near_plane();
        float far_clip = s_scene_renderer_data.m_camera->get_far_plane();
        float clip_range = far_clip - near_clip;

        // Calculate orthographic projection matrix for each cascade
        float last_split_dist = 0.0;

        for (uint32_t i = 0; i < NUM_CASCADES; ++i)
        {
            float split_dist = s_scene_renderer_data.m_csm_shadows.m_cascade_splits[i];

            glm::vec3 frustum_corners[8] = {
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),
            };

            // Project frustum corners into world space
            glm::mat4 inv_cam = glm::inverse(
                s_scene_renderer_data.m_camera->get_view_projection());
            for (uint32_t i = 0; i < 8; ++i)
            {
                glm::vec4 corner_world_space = inv_cam * glm::vec4(frustum_corners[i], 1.0f);
                frustum_corners[i] = corner_world_space / corner_world_space.w;
            }

            for (uint32_t i = 0; i < 4; ++i)
            {
                glm::vec3 dist = frustum_corners[i + 4] - frustum_corners[i];
                frustum_corners[i + 4] = frustum_corners[i] + (dist * split_dist);
                frustum_corners[i] = frustum_corners[i] + (dist * last_split_dist);
            }

            // Calc frustum center
            glm::vec3 frustum_center = glm::vec3(0.0f);
            for (uint32_t i = 0; i < 8; ++i)
            {
                frustum_center += frustum_corners[i];
            }
            frustum_center /= 8.0f;

            float radius = 0.0f;
            for (uint32_t i = 0; i < 8; ++i)
            {
                float distance = glm::length(frustum_corners[i] - frustum_center);
                radius = glm::max(radius, distance);
            }
            radius = ceilf(radius * 16.0f) / 16.0f;

            glm::vec3 max_extents = glm::vec3(radius);
            glm::vec3 min_extents = -max_extents;

            glm::mat4 light_view_matrix = glm::lookAt(
                frustum_center - s_scene_renderer_data.m_directional_light.direction * -min_extents.z, frustum_center,
                glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 light_ortho_matrix = glm::ortho(min_extents.x, max_extents.x, min_extents.y, max_extents.y, 0.0f,
                                                      max_extents.z - min_extents.z);

            float split_depth = (s_scene_renderer_data.m_camera->get_near_plane() + split_dist * clip_range) * -1.0f;
            s_scene_renderer_data.m_csm_shadows.m_cascade_splits[i] = split_depth;

            s_scene_renderer_data.m_csm_shadows.m_avg_frustum_size = glm::max(
                s_scene_renderer_data.m_csm_shadows.m_avg_frustum_size, max_extents.x - min_extents.x);

            s_scene_renderer_data.m_csm_shadows.m_dir_light_view_matrices[i] = light_view_matrix;
            s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection_matrices[i] = light_ortho_matrix *
                light_view_matrix;
            s_scene_renderer_data.m_csm_shadows.m_dir_shadow_frustum_planes[i] =
                glm::vec2(min_extents.z, max_extents.z);

            {
                glm::vec4 shadow_origin = glm::vec4(0.0, 0.0, 0.0, 1.0);
                shadow_origin = s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection_matrices[i] *
                    shadow_origin;
                shadow_origin = shadow_origin * (s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.x /
                    2.0f);

                glm::vec4 rounded_origin = glm::round(shadow_origin);
                glm::vec4 round_offset = rounded_origin - shadow_origin;
                round_offset = round_offset * (2.0f / s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.x);
                round_offset.z = 0.0f;
                round_offset.w = 0.0f;

                glm::mat4& shadow_proj = light_ortho_matrix;
                shadow_proj[3] += round_offset;

                s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection_matrices[i] = shadow_proj *
                    light_view_matrix;
            }

            last_split_dist = split_dist;
        }
    }

    void scene_renderer::create_shadow_fbo(uint32_t width, uint32_t height)
    {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps);
        glTextureStorage3D(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, 1, GL_DEPTH_COMPONENT32F, width,
                           height, NUM_CASCADES);

        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_BORDER);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_BORDER);

        constexpr float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTextureParameterfv(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, GL_TEXTURE_BORDER_COLOR,
                             bordercolor);

        glCreateFramebuffers(1, &s_scene_renderer_data.m_csm_shadows.m_shadow_fbo);
        glNamedFramebufferTexture(s_scene_renderer_data.m_csm_shadows.m_shadow_fbo, GL_DEPTH_ATTACHMENT,
                                  s_scene_renderer_data.m_csm_shadows.m_dir_shadow_maps, 0);

        GLenum draw_buffers[] = {GL_NONE};
        glNamedFramebufferDrawBuffers(s_scene_renderer_data.m_csm_shadows.m_shadow_fbo, 1, draw_buffers);
        glNamedFramebufferReadBuffer(s_scene_renderer_data.m_csm_shadows.m_shadow_fbo, GL_NONE);

        int status = glCheckNamedFramebufferStatus(s_scene_renderer_data.m_csm_shadows.m_shadow_fbo, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        }
    }

    void scene_renderer::generate_shadow_map(uint32_t width, uint32_t height)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, s_scene_renderer_data.m_csm_shadows.m_shadow_fbo);
        glViewport(0, 0, width, height);
        glClear(GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_FRONT);

        update_csm_splits();
        update_csm_frusta();

        s_scene_renderer_data.m_csm_shadows_shader->bind();

        s_scene_renderer_data.m_csm_shadows_shader->set_mat4("u_light_view_projections[0]",
                                                             s_scene_renderer_data.m_csm_shadows.
                                                             m_dir_light_view_projection_matrices[0]);
        s_scene_renderer_data.m_csm_shadows_shader->set_mat4("u_light_view_projections[1]",
                                                             s_scene_renderer_data.m_csm_shadows.
                                                             m_dir_light_view_projection_matrices[1]);
        s_scene_renderer_data.m_csm_shadows_shader->set_mat4("u_light_view_projections[2]",
                                                             s_scene_renderer_data.m_csm_shadows.
                                                             m_dir_light_view_projection_matrices[2]);

        const auto view = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
            entt::get<name_component, transform_component>);

        for (auto&& [actor, model_renderer, name, transform] : view.each())
        {
            s_scene_renderer_data.m_csm_shadows_shader->set_mat4("m_model", transform.get_transform_matrix());
            for (const auto& renderable : model_renderer.model->get_model_renderables())
            {
                renderer::submit_command({
                    s_scene_renderer_data.m_csm_shadows_shader,
                    renderable->get_vertex_array_buffer(),
                    nullptr,
                });
            }
        }
        glCullFace(GL_BACK);
    }

    GLuint scene_renderer::generate_random_angles_texture_3d(uint32_t size)
    {
        int buffer_size = size * size * size;
        auto data = std::make_unique<glm::vec2[]>(buffer_size); // glm::vec2[buffer_size];

        for (int z = 0; z < size; ++z)
        {
            for (int y = 0; y < size; ++y)
            {
                for (int x = 0; x < size; ++x)
                {
                    int index = x + y * size + z * size * size;
                    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
                    static std::mt19937 generator;
                    float random_angle = glm::two_pi<double>() * distribution(generator);
                    // Random angles in range [0, 2PI);
                    data[index] = glm::vec2(glm::cos(random_angle), glm::sin(random_angle));
                    // Map sine and cosine values to [0, 1] range
                }
            }
        }

        GLuint tex_id = 0;
        glCreateTextures(GL_TEXTURE_3D, 1, &tex_id);
        glTextureStorage3D(tex_id, 1, GL_RG32F, size, size, size);
        glTextureSubImage3D(tex_id, 0, 0, 0, 0, size, size, size, GL_RG, GL_FLOAT, data.get());

        glTextureParameteri(tex_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(tex_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(tex_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(tex_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(tex_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return tex_id;
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
            "Assets/Shaders/Shadows/Shadows.vert", "Assets/Shaders/Shadows/Shadows.frag");

        s_scene_renderer_data.m_csm_shadows_shader = shader::create(
            "Assets/Shaders/Shadows/GenerateCSM.vert", "Assets/Shaders/Shadows/GenerateCSM.frag",
            "Assets/Shaders/Shadows/GenerateCSM.geom");
    }

    void scene_renderer::generate_frame_buffers()
    {
        s_scene_renderer_data.m_geometry_frame_buffer = frame_buffer::create({
            1920, 1080,
            {
                {"position", frame_buffer_attachment_format::rgba16f},
                {"albedo", frame_buffer_attachment_format::rgba16f},
                {"normal", frame_buffer_attachment_format::rgba16f},
                {"rough-meta-ao", frame_buffer_attachment_format::rgba16f}
            }
        });

        s_scene_renderer_data.m_shadow_frame_buffer = frame_buffer::create({
            1920, 1080, {
                {
                    "shadow", frame_buffer_attachment_format::rgba16f
                }
            }
        });

        s_scene_renderer_data.m_final_frame_buffer =
            frame_buffer::create({1920, 1080, {{"final", frame_buffer_attachment_format::rgba16f}}});
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
        s_scene_renderer_data.m_lights_ubo = uniform_buffer::create(
            sizeof(point_light_data) + sizeof(directional_light_data), 1);
    }

    void scene_renderer::setup_environment()
    {
        const shared<texture_cubemap> sky_cubemap = texture_cubemap::create({
            "Assets/Textures/HDR/drakensberg_solitary_mountain_4k.hdr",
            texture_filtering::linear,
            texture_wrapping::clamp_edge
        });
        s_scene_renderer_data.m_lighting_environment = lighting_environment::create(
            {sky_cubemap, 2048, 512, 2048, 2048});
    }
}
