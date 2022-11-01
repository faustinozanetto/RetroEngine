#include "editor_layer.h"

#include "core/application/retro_application.h"
#include "core/input/input_manager.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "renderer/lighting/directional_light.h"
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
            metals->add_component<material_component>(metalsMat);
            auto& transform = metals->add_component<transform_component>();
            transform.scale = glm::vec3(0.01f);
            transform.position = {0.0f, -0.4f, 9.0f};
        } 
        auto light = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                          create_actor();
        light->add_component<name_component>("Light");
        auto directional_light = create_shared<renderer::directional_light>();
        light->add_component<light_renderer_component>(directional_light, light_type::directional);
        light->add_component<model_renderer_component>(
            renderer::model::create("Assets/Models/Cube.obj"));
        light->add_component<transform_component>();

        auto floor = retro_application::get_application().get_scene_manager()->get_active_scene()->
                                                          create_actor();
        floor->add_component<name_component>("Floor");
        floor->add_component<model_renderer_component>(
            renderer::model::create("Assets/Models/Cube.obj"));
        floor->add_component<transform_component>();
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

        auto cam_pos = m_camera->get_focal_point();
        if (input::input_manager::is_key_pressed(input::key::W))
            cam_pos += m_camera->get_forward_direction() * PanSpeed().first;
        if (input::input_manager::is_key_pressed(input::key::S))
            cam_pos -= m_camera->get_forward_direction() * PanSpeed().first;
        if (input::input_manager::is_key_pressed(input::key::A))
            cam_pos -= m_camera->get_right_direction() * PanSpeed().second;
        if (input::input_manager::is_key_pressed(input::key::D))
            cam_pos += m_camera->get_right_direction() * PanSpeed().second;
        if (input::input_manager::is_key_pressed(input::key::Q))
            cam_pos += m_camera->get_up_direction() * PanSpeed().second;
        if (input::input_manager::is_key_pressed(input::key::E))
            cam_pos -= m_camera->get_up_direction() * PanSpeed().second;
        m_camera->set_focal_point(cam_pos);
        renderer::scene_renderer::end_render();
    }

    void editor_layer::initialize_editor_scene()
    {
        const shared<scene> scene = scene::create("Editor Scene");
        retro_application::get_application().get_scene_manager()->set_active_scene(scene);
        m_camera = retro::create_shared<renderer::camera>(50.0f, 0.01f, 2000.0f);
    }

    std::pair<float, float> editor_layer::PanSpeed() const
    {
        float x = std::min(renderer::scene_renderer::get_geometry_frame_buffer()->get_width() / 1000.0f, 2.4f);
        // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(renderer::scene_renderer::get_geometry_frame_buffer()->get_height() / 1000.0f, 2.4f);
        // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor * 0.25f, yFactor * 0.25f};
    }
}
