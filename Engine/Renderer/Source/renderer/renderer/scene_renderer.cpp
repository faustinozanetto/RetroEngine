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

        s_scene_renderer_data.m_csm_shadows.m_dir_shadow_frustum_planes = glm::vec2(120, 250);
        s_scene_renderer_data.m_csm_shadows.m_dir_shadow_frustum_size = 20.0f;
        s_scene_renderer_data.m_csm_shadows.m_light_radius_uv = 0.0f;
        s_scene_renderer_data.m_csm_shadows.scene_size = glm::uvec2(10);

        s_scene_renderer_data.shadows_ubo = uniform_buffer::create(sizeof(shadows_data), 4);


        s_scene_renderer_data.shadows_data.light_view_projection = s_scene_renderer_data.m_csm_shadows.
            m_dir_light_view_projection;
        s_scene_renderer_data.shadows_data.light_view = s_scene_renderer_data.m_csm_shadows.m_dir_light_view;
        s_scene_renderer_data.shadows_ubo->set_data(&s_scene_renderer_data.shadows_data, sizeof(shadows_data));

        update_light_matrices();

        s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res = glm::uvec2(8196);
        create_directional_shadow_map(s_scene_renderer_data.m_csm_shadows.m_dir_light_shadow_map_res.x,
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
        ImGui::SliderFloat2("Scene Size", glm::value_ptr(s_scene_renderer_data.m_csm_shadows.scene_size), 0, 50);
        ImGui::SliderInt("PCF Samples", &s_scene_renderer_data.m_csm_shadows.m_pcf_filter_samples, 0, 128);
        ImGui::SliderInt("Blocker Samples", &s_scene_renderer_data.m_csm_shadows.m_blocker_search_samples, 0, 128);

        ImGui::End();

        update_light_matrices();
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
        const auto lights = s_scene_renderer_data.m_scene->get_actor_registry().view<
            light_renderer_component, name_component, transform_component>();
        for (auto&& [actor, light, name, transform] : lights.each())
        {
            if (light.type == light_type::point)
            {
                auto point_light = dynamic_cast<retro::renderer::point_light*>(light.light.get());
                s_scene_renderer_data.m_pointLight.position = glm::vec4(transform.position, 0);
                s_scene_renderer_data.m_pointLight.color = glm::vec4(point_light->get_color(), 0);
                s_scene_renderer_data.m_pointLight.intensity = point_light->get_intensity();
                s_scene_renderer_data.m_pointLight.radius = point_light->get_radius();
                point_light = nullptr;
            }
            else if (light.type == light_type::directional)
            {
                auto directional_light = dynamic_cast<retro::renderer::directional_light*>(light.light.get());
                s_scene_renderer_data.m_directional_light.direction = glm::vec4(directional_light->get_direction(), 0);
                s_scene_renderer_data.m_directional_light.color = glm::vec4(directional_light->get_color(), 0);
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

        s_scene_renderer_data.shadows_data.light_view_projection = s_scene_renderer_data.m_csm_shadows.
            m_dir_light_view_projection;
        s_scene_renderer_data.shadows_data.light_view = s_scene_renderer_data.m_csm_shadows.m_dir_light_view;
        s_scene_renderer_data.shadows_ubo->set_data(&s_scene_renderer_data.shadows_data, sizeof(shadows_data));

        /* ==================== SHADOW PASS ==================== */
        s_scene_renderer_data.m_shadow_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, true);
        renderer::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});
        s_scene_renderer_data.m_csm_shadows_shader->bind();
        renderer::clear_screen();
        glCullFace(GL_FRONT);
        for (auto&& [actor, model_renderer, name, transform] : view.each())
        {
            for (const auto& renderable : model_renderer.model->get_model_renderables())
            {
                s_scene_renderer_data.m_csm_shadows_shader->set_mat4("u_model", transform.get_transform_matrix());
                renderer::submit_command({
                    s_scene_renderer_data.m_csm_shadows_shader,
                    renderable->get_vertex_array_buffer(),
                    nullptr,
                });
            }
        }
        glCullFace(GL_BACK);
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

        /*==================== LIGHTING PASS ==================== */
        s_scene_renderer_data.m_final_frame_buffer->bind();
        renderer::set_renderer_state(renderer_state::depth_test, false);
        s_scene_renderer_data.m_lighting_shader->bind();

        s_scene_renderer_data.m_lighting_shader->set_int("u_blocker_search_samples",
                                                         s_scene_renderer_data.m_csm_shadows.m_blocker_search_samples);
        s_scene_renderer_data.m_lighting_shader->set_int("u_pcf_samples",
                                                         s_scene_renderer_data.m_csm_shadows.m_pcf_filter_samples);
        s_scene_renderer_data.m_lighting_shader->set_float("u_light_radius_uv",
                                                           s_scene_renderer_data.m_csm_shadows.m_light_radius_uv / (
                                                               s_scene_renderer_data.m_csm_shadows.
                                                               m_dir_shadow_frustum_size * 2.0f));
        s_scene_renderer_data.m_lighting_shader->set_float("u_light_near",
                                                           s_scene_renderer_data.m_csm_shadows.
                                                           m_dir_shadow_frustum_planes.x);
        s_scene_renderer_data.m_lighting_shader->set_float("u_light_far",
                                                           s_scene_renderer_data.m_csm_shadows.
                                                           m_dir_shadow_frustum_planes.y);

        s_scene_renderer_data.m_lighting_shader->set_float("pointLight.radius",
                                                           s_scene_renderer_data.m_pointLight.radius);
        s_scene_renderer_data.m_lighting_shader->set_float("pointLight.intensity",
                                                           s_scene_renderer_data.m_pointLight.intensity);
        s_scene_renderer_data.m_lighting_shader->set_vec_float4("pointLight.color",
                                                                s_scene_renderer_data.m_pointLight.color);
        s_scene_renderer_data.m_lighting_shader->set_vec_float4("pointLight.position",
                                                                s_scene_renderer_data.m_pointLight.position);

        s_scene_renderer_data.m_lighting_shader->set_float("directionalLight.intensity",
                                                           s_scene_renderer_data.m_directional_light.intensity);
        s_scene_renderer_data.m_lighting_shader->set_vec_float4("directionalLight.color",
                                                                s_scene_renderer_data.m_directional_light.color);
        s_scene_renderer_data.m_lighting_shader->set_vec_float4("directionalLight.direction",
                                                                s_scene_renderer_data.m_directional_light.direction);

        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(0), 0); // position
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(1), 1); // albedo
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(2), 2); // normal
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(3), 3);
        // rough - metal - ao
        renderer::bind_texture(s_scene_renderer_data.m_geometry_frame_buffer->get_attachment_id(4), 4); // view position
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_irradiance_texture(), 5);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_prefilter_texture(), 6);
        renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_brdf_lut_texture(), 7);
        glBindSampler(9, s_scene_renderer_data.m_csm_shadows.m_pcf_sampler);
        renderer::bind_texture(s_scene_renderer_data.m_shadow_frame_buffer->get_depth_attachment_id(), 8);
        renderer::bind_texture(s_scene_renderer_data.m_shadow_frame_buffer->get_depth_attachment_id(), 9);
        renderer::bind_texture(s_scene_renderer_data.m_csm_shadows.m_random_angles_tex3d_id, 10);
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

    scene_renderer_data& scene_renderer::get_data()
    {
        return s_scene_renderer_data;
    }

    uint32_t scene_renderer::get_final_texture()
    {
        return s_scene_renderer_data.m_final_frame_buffer->get_attachment_id(0);
    }

    void scene_renderer::create_directional_shadow_map(uint32_t width, uint32_t height)
    {
        GLfloat border[] = {1.0, 0.0, 0.0, 0.0};

        glCreateTextures(GL_TEXTURE_2D, 1, &s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map);
        glTextureStorage2D(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, 1, GL_DEPTH_COMPONENT32F, width,
                           height);

        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_BORDER);
        glTextureParameteri(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_BORDER);
        glTextureParameterfv(s_scene_renderer_data.m_csm_shadows.m_dir_shadow_map, GL_TEXTURE_BORDER_COLOR, border);
    }


    void scene_renderer::update_light_matrices()
    {
        glm::vec3 light_dir = s_scene_renderer_data.m_directional_light.direction;

        glm::vec3 max_extents = glm::vec3(s_scene_renderer_data.m_csm_shadows.scene_size.x);
        glm::vec3 min_extents = glm::vec3(-s_scene_renderer_data.m_csm_shadows.scene_size.y);
        glm::vec3 scene_center = (max_extents + min_extents) * 0.5f;

        glm::mat4 light_view_matrix = glm::lookAt(scene_center - light_dir * -min_extents.z, scene_center,
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 light_ortho_matrix = glm::ortho(min_extents.x, max_extents.x, min_extents.y, max_extents.y, 0.0f,
                                                  max_extents.z - min_extents.z);

        s_scene_renderer_data.m_csm_shadows.m_dir_shadow_frustum_size = max_extents.x - min_extents.x;

        s_scene_renderer_data.m_csm_shadows.m_dir_light_view = light_view_matrix;
        s_scene_renderer_data.m_csm_shadows.m_dir_light_view_projection = light_ortho_matrix * light_view_matrix;
        s_scene_renderer_data.m_csm_shadows.m_dir_shadow_frustum_planes = glm::vec2(min_extents.z, max_extents.z);
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
            "Assets/Shaders/Shadows/GeneratePCSS.vert", "Assets/Shaders/Shadows/GeneratePCSS.frag"
        );
    }

    void scene_renderer::generate_frame_buffers()
    {
        texture_specification geom_tex_spec = {
            glm::uvec2(1920, 1080),
            texture_filtering::linear,
            texture_wrapping::clamp_edge,
            GL_RGBA,GL_RGB16F
        };
        s_scene_renderer_data.m_geometry_frame_buffer = frame_buffer::create({
            1920, 1080, {
                {"position", geom_tex_spec},
                {"albedo", geom_tex_spec},
                {"normal", geom_tex_spec},
                {"rough-meta-ao", geom_tex_spec},
                {"viewPosition", geom_tex_spec}
            }
        });

        texture_specification shadow_tex_spec = {
            glm::uvec2(4096, 4096),
            texture_filtering::nearest,
            texture_wrapping::clamp_border,
            GL_DEPTH_COMPONENT32F,
            GL_RGB16F
        };
        s_scene_renderer_data.m_shadow_frame_buffer = frame_buffer::create({
            4096, 4096, {
                {
                    "shadow", {
                        shadow_tex_spec
                    }
                }
            }
        });

        texture_specification final_tex_spec = {
            glm::uvec2(1920, 1080),
            texture_filtering::linear,
            texture_wrapping::clamp_edge,
            GL_RGBA,GL_RGB16F
        };
        s_scene_renderer_data.m_final_frame_buffer =
            frame_buffer::create({
                1920, 1080, {
                    {"final", final_tex_spec}
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
        s_scene_renderer_data.m_pointLight.color = glm::vec4(0);
        s_scene_renderer_data.m_pointLight.position = glm::vec4(0);
        s_scene_renderer_data.m_pointLight.radius = 0;
        s_scene_renderer_data.m_pointLight.intensity = 0;

        s_scene_renderer_data.m_directional_light.color = glm::vec4(0);
        s_scene_renderer_data.m_directional_light.direction = glm::vec4(0);
        s_scene_renderer_data.m_directional_light.intensity = 0;

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
