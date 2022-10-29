#include "editor_layer.h"

#include "core/application/retro_application.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "renderer/materials/material.h"
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
        auto roughness = renderer::texture::create({
            "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_MetallicRoughness.png",
            renderer::texture_filtering::linear,
            renderer::texture_wrapping::clamp_edge,
        });
        renderer::material_texture roughnessTexture = {
            roughness, true
        };
        auto metallic = renderer::texture::create({
            "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_MetallicRoughness.png",
            renderer::texture_filtering::linear,
            renderer::texture_wrapping::clamp_edge,
        });
        renderer::material_texture metallicTexture = {
            metallic, true
        };
        auto ambient_occlusion = renderer::texture::create({
            "Assets/Models/SciFiHelmet/glTF/SciFiHelmet_AmbientOcclusion.png",
            renderer::texture_filtering::linear,
            renderer::texture_wrapping::clamp_edge,
        });
        renderer::material_texture ambient_occlusion_texture = {
            ambient_occlusion, true
        };
        const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
            {renderer::material_texture_type::albedo, albedoTexture},
            {renderer::material_texture_type::normal, normalTexture},
            {renderer::material_texture_type::metallic, roughnessTexture},
            {renderer::material_texture_type::roughness, metallicTexture},
            {renderer::material_texture_type::ambient_occlusion, ambient_occlusion_texture}
        };
        const renderer::material_specification materialSpecification = {
            textures,
            glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
            1.0f,
            1.0f,
        };
        auto material = renderer::material::create(
            materialSpecification);
        auto sphere = retro_application::get_application().get_scene_manager()->get_active_scene()->create_actor();
        sphere->add_component<name_component>("SciFiHelmet");
        auto sphereModel = renderer::model::create("Assets/Models/SciFiHelmet/glTF/SciFiHelmet.obj");
        sphere->add_component<model_renderer_component>(sphereModel);
        sphere->add_component<material_component>(material);
        sphere->add_component<transform_component>();
    }

    void editor_layer::on_layer_unregistered()
    {
    }

    void editor_layer::on_layer_updated()
    {
        renderer::scene_renderer::begin_render();

        renderer::scene_renderer::end_render();
    }

    void editor_layer::initialize_editor_scene()
    {
        const shared<scene> scene = scene::create("Editor Scene");
        retro_application::get_application().get_scene_manager()->set_active_scene(scene);
        m_camera = retro::create_shared<renderer::camera>(50.0f, 0.01f, 2000.0f);
    }
}
