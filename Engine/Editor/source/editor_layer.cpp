#include "editor_layer.h"

#include "core/application/retro_application.h"
#include "core/input/input_manager.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "renderer/lighting/directional_light.h"
#include "renderer/lighting/point_light.h"
#include "renderer/materials/material.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
    editor_layer::editor_layer(const std::string& name) : layer(name)
    {
    }

    editor_layer::~editor_layer()
    {
    }

    void editor_layer::on_layer_registered()
    {
        initialize_editor_scene();
        renderer::scene_renderer::set_scene(
            retro_application::get_application().get_scene_manager()->get_active_scene());
        renderer::scene_renderer::initialize(m_camera);

        /*
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto other = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_OcclusionRoughMetal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture otherTexture = {
                        other, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, otherTexture},
                        {renderer::material_texture_type::roughness, otherTexture},
                        {renderer::material_texture_type::ambient_occlusion, otherTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto baseWoodenMaterial = renderer::material::create(
                        materialSpecification);
                    auto baseWooden = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                           create_actor();
                    baseWooden->add_component<name_component>("FighterHelmet Base");
                    baseWooden->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/WoodenBase.fbx"));
                    baseWooden->add_component<material_component>(baseWoodenMaterial);
                    auto& transform = baseWooden->add_component<transform_component>();
                    transform.scale = glm::vec3(0.01f);
                    transform.position = {0.0f, -0.4f, 9.0f};
        
                    auto pipe = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                     create_actor();
                    pipe->add_component<name_component>("FighterHelmet Pipe");
                    pipe->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/Pipe.fbx"));
                    pipe->add_component<material_component>(baseWoodenMaterial);
                    auto& transform2 = pipe->add_component<transform_component>();
                    transform2.scale = glm::vec3(0.01f);
                    transform2.position = {0.0f, -0.4f, 9.0f};
                }
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto other = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_OcclusionRoughMetal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture otherTexture = {
                        other, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, otherTexture},
                        {renderer::material_texture_type::roughness, otherTexture},
                        {renderer::material_texture_type::ambient_occlusion, otherTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto maskMat = renderer::material::create(
                        materialSpecification);
                    auto mask = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                     create_actor();
                    mask->add_component<name_component>("FighterHelmet Mask");
                    mask->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/Mask.fbx"));
                    mask->add_component<material_component>(maskMat);
                    auto& transform = mask->add_component<transform_component>();
                    transform.scale = glm::vec3(0.01f);
                    transform.position = {0.0f, -0.4f, 9.0f};
                }
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto other = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_OcclusionRoughMetal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture otherTexture = {
                        other, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, otherTexture},
                        {renderer::material_texture_type::roughness, otherTexture},
                        {renderer::material_texture_type::ambient_occlusion, otherTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto helmetMat = renderer::material::create(
                        materialSpecification);
                    auto helmet = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                       create_actor();
                    helmet->add_component<name_component>("FighterHelmet Helmet");
                    helmet->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/Helmet.fbx"));
                    helmet->add_component<material_component>(helmetMat);
                    auto& transform = helmet->add_component<transform_component>();
                    transform.scale = glm::vec3(0.01f);
                    transform.position = {0.0f, -0.4f, 9.0f};
                }
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto other = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_OcclusionRoughMetal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture otherTexture = {
                        other, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, otherTexture},
                        {renderer::material_texture_type::roughness, otherTexture},
                        {renderer::material_texture_type::ambient_occlusion, otherTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto lensesMat = renderer::material::create(
                        materialSpecification);
                    auto lenses = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                       create_actor();
                    lenses->add_component<name_component>("FighterHelmet Lenses");
                    lenses->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/Lenses.fbx"));
                    lenses->add_component<material_component>(lensesMat);
                    auto& transform = lenses->add_component<transform_component>();
                    transform.scale = glm::vec3(0.01f);
                    transform.position = {0.0f, -0.4f, 9.0f};
                }
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto other = renderer::texture::create({
                        "Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_OcclusionRoughMetal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture otherTexture = {
                        other, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, otherTexture},
                        {renderer::material_texture_type::roughness, otherTexture},
                        {renderer::material_texture_type::ambient_occlusion, otherTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto metalsMat = renderer::material::create(
                        materialSpecification);
                    auto metals = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                       create_actor();
                    metals->add_component<name_component>("FighterHelmet Metals");
                    metals->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/FlightHelmet/glTF/MetalParts.fbx"));
                    auto& mat = metals->add_component<material_component>(metalsMat);
                    auto& transform = metals->add_component<transform_component>();
                    transform.scale = glm::vec3(0.01f);
                    transform.position = {0.0f, -0.4f, 9.0f};
                    mat.material->serialize();
                }
        
                /*
                
                {
                    auto albedo = renderer::texture::create({
                        "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_BaseColor.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture albedoTexture = {
                        albedo, true
                    };
                    auto normal = renderer::texture::create({
                        "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_Normal.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture normalTexture = {
                        normal, true
                    };
                    auto metalRough = renderer::texture::create({
                        "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_MetallicRoughness.png",
                        renderer::texture_filtering::linear,
                        renderer::texture_wrapping::clamp_edge,
                    });
                    renderer::material_texture metalRoughTexture = {
                        metalRough, true
                    };
                    auto ao = renderer::texture::create({
                     "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_AmbientOcclusion.png",
                     renderer::texture_filtering::linear,
                     renderer::texture_wrapping::clamp_edge,
                 });
                    renderer::material_texture aoTexture = {
                        ao, true
                    };
                    const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                        {renderer::material_texture_type::albedo, albedoTexture},
                        {renderer::material_texture_type::normal, normalTexture},
                        {renderer::material_texture_type::metallic, metalRoughTexture},
                        {renderer::material_texture_type::roughness, metalRoughTexture},
                        {renderer::material_texture_type::ambient_occlusion, aoTexture}
                    };
                    const renderer::material_specification materialSpecification = {
                        textures,
                        glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
                        1.0f,
                        1.0f,
                    };
                    auto metalsMat = renderer::material::create(
                        materialSpecification);
                    auto metals = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                       create_actor();
                    metals->add_component<name_component>("SciFi Helmet");
                    metals->add_component<model_renderer_component>(
                        renderer::model::create("Assets/Models/SciFiHelmet/glTF/SciFiHelmet.gltf"));
                    metals->add_component<material_component>(metalsMat);
                    auto& transform = metals->add_component<transform_component>();
                }
                */
             

        /*
                auto sponza = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                     create_actor();
                sponza->add_component<name_component>("Sponza");
                sponza->add_component<model_renderer_component>(
                    renderer::model::create("Assets/Models/Sponza/Meshes/Sponza_Modular.FBX"));
                sponza->add_component<transform_component>().scale = glm::vec3(0.01f);
                */
        {
            auto light = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                              create_actor();
            light->add_component<name_component>("Point Light");
            auto point_light = create_shared<renderer::point_light>();
            light->add_component<light_renderer_component>(point_light, light_type::point);
            light->add_component<model_renderer_component>(
                renderer::model::create("Assets/Models/Cube.obj"));
            auto& light_trans = light->add_component<transform_component>();
            light_trans.position = {0.0f, 0.4f, 9.3f};
            light_trans.scale = {0.1f, 0.1f, 0.1f};
        }
        {
            auto light = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                              create_actor();
            light->add_component<name_component>("Directional Light");
            auto dir_light = create_shared<renderer::directional_light>();
            light->add_component<light_renderer_component>(dir_light, light_type::directional);
            light->add_component<transform_component>();
        }
        m_camera->set_position({0.0f, 0.0f, 10.0f});

        {
            auto floor = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                              create_actor();
            floor->add_component<name_component>("Floor");
            floor->add_component<model_renderer_component>(
                renderer::model::create("Assets/Models/Plane.obj"));
            auto& floor_trans = floor->add_component<transform_component>();
            floor_trans.position = {0.0f, 0.4f, 0.0f};
            floor_trans.scale = {20.0f, 1.0f, 20.0f};
        }
   
        {
            float hk_unit_scale_factor = 1.25f;
            float hk_radius = hk_unit_scale_factor;
            float space_size = 0.5 * hk_radius;

            glm::ivec2 grid_size = {3, 3};
            glm::vec3 offset = glm::vec3(hk_radius + space_size);
            glm::vec3 start_pos = glm::vec3(-offset.x * float(grid_size.x / 2), 0.0,
                                            -offset.y * float(grid_size.y / 2));

            const shared<renderer::model>& sphere_model = renderer::model::create("Assets/Models/Monkey.fbx");

            renderer::material_texture albedoTexture = {
                nullptr, false
            };

            renderer::material_texture normalTexture = {
                nullptr, false
            };

            renderer::material_texture metalRoughTexture = {
                nullptr, false
            };

            renderer::material_texture aoTexture = {
                nullptr, false
            };
            const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
                {renderer::material_texture_type::albedo, albedoTexture},
                {renderer::material_texture_type::normal, normalTexture},
                {renderer::material_texture_type::metallic, metalRoughTexture},
                {renderer::material_texture_type::roughness, metalRoughTexture},
                {renderer::material_texture_type::ambient_occlusion, aoTexture}
            };

            const renderer::material_specification materialSpecification = {
                textures,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                1.0f,
                1.0f,
                1.0f,
            };
            auto material = renderer::material::create(
                materialSpecification);

            for (uint32_t y = 0; y < grid_size.y; ++y)
            {
                for (uint32_t x = 0; x < grid_size.x; ++x)
                {
                    glm::vec3 position = start_pos + glm::vec3(x, 0.5f, y) * offset;
                    auto sphere = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                                       create_actor();
                    sphere->add_component<name_component>(
                        "Sphere (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                    sphere->add_component<model_renderer_component>(sphere_model);
                    sphere->add_component<material_component>(material);
                    auto& transform = sphere->add_component<transform_component>();
                    transform.position = position;
                    transform.scale = glm::vec3(0.005f);
                }
            }
        }
        
    }

    void editor_layer::on_layer_unregistered()
    {
    }

    void editor_layer::on_layer_updated()
    {
        renderer::scene_renderer::begin_render();

        float time = renderer::renderer::get_time();
        //  auto registry = retro_application::get_application().get_scene_manager()->get_active_scene()->
        //                                                     get_actor_registry().view<transform_component>();
        /*
                for (auto& actor : registry)
                {
                    if (!retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                              get_actor_registry().has<light_renderer_component>(actor))
                    {
                        auto& transform = registry.get<transform_component>(actor);
                        transform.rotation = {0.0f, glm::sin(time) * 2.0f, 0.0f};
                    }
                }
                */


        float cam_mov_amount = 0.05f;
        if (input::input_manager::is_key_pressed(input::key::W))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 0, -1),
                           cam_mov_amount);
        if (input::input_manager::is_key_pressed(input::key::S))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 0, 1),
                           cam_mov_amount);
        if (input::input_manager::is_key_pressed(input::key::A))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(-1, 0, 0),
                           cam_mov_amount);
        if (input::input_manager::is_key_pressed(input::key::D))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(1, 0, 0),
                           cam_mov_amount);
        if (input::input_manager::is_key_pressed(input::key::Q))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 1, 0),
                           cam_mov_amount);
        if (input::input_manager::is_key_pressed(input::key::E))
            m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, -1, 0),
                           cam_mov_amount);

        if (input::input_manager::is_mouse_button_pressed(input::key::ButtonRight))
        {
            if (!m_is_mouse_move)
            {
                m_mouse_pressed_position = input::input_manager::get_mouse_location();
            }
            m_is_mouse_move = true;
        }
        else
        {
            m_is_mouse_move = false;
        }
        if (m_is_mouse_move)
        {
            auto mouse_pos = input::input_manager::get_mouse_location();
            auto delta_pos = mouse_pos - m_mouse_pressed_position;

            auto rot_y = delta_pos.x != 0.0f;
            auto rot_x = delta_pos.y != 0.0f;

            /* pitch */
            if (rot_x)
            {
                m_camera->set_orientation(
                    glm::angleAxis(glm::radians(delta_pos.y * 0.2f), glm::vec3(1, 0, 0)) * m_camera->get_orientation());
            }

            /* yaw */
            if (rot_y)
            {
                m_camera->set_orientation(
                    m_camera->get_orientation() * glm::angleAxis(glm::radians(delta_pos.x * 0.2f), glm::vec3(0, 1, 0)));
            }

            if (rot_x || rot_y)
            {
                m_mouse_pressed_position = input::input_manager::get_mouse_location();
            }
        }
        renderer::scene_renderer::end_render();
    }

    void editor_layer::initialize_editor_scene()
    {
        const shared<scene> scene = scene::create("Editor Scene");
        retro_application::get_application().get_scene_manager()->set_active_scene(scene);
        m_camera = retro::create_shared<renderer::camera>(50.0f, 0.01f, 200.0f);
    }
}
