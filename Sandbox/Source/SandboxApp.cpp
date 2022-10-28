#pragma once

#include <ranges>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "core/application/retro_application.h"
#include "core/interfaces/interface_layer.h"
#include "core/layers/layer.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "core/entry_point.h"
#include "renderer/camera/camera.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"


class sandbox_layer : public retro::layer
{
public:
    sandbox_layer() : layer("Sandbox Layer")
    {
        m_Model = retro::renderer::model::create("Assets/Models/Cerberus/source/Cerberus_LP.FBX.fbx");

        m_LightModel = retro::renderer::model::create("Assets/Models/Cube.obj");

        auto albedo = retro::renderer::texture::create({
            "Assets/Models/Cerberus/textures/Cerberus_A.png",
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_edge,
        });
        retro::renderer::material_texture albedoTexture = {
            albedo, true
        };
        auto normal = retro::renderer::texture::create({
            "Assets/Models/Cerberus/textures/Cerberus_N.png",
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_edge,
        });
        retro::renderer::material_texture normalTexture = {
            normal, true
        };
        auto roughness = retro::renderer::texture::create({
            "Assets/Models/Cerberus/textures/Cerberus_R.png",
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_edge,
        });
        retro::renderer::material_texture roughnessTexture = {
            roughness, true
        };
        auto metallic = retro::renderer::texture::create({
            "Assets/Models/Cerberus/textures/Cerberus_M.png",
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_edge,
        });
        retro::renderer::material_texture metallicTexture = {
            metallic, true
        };
        const std::map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures = {
            {retro::renderer::material_texture_type::albedo, albedoTexture},
            {retro::renderer::material_texture_type::normal, normalTexture},
            {retro::renderer::material_texture_type::metallic, roughnessTexture},
            {retro::renderer::material_texture_type::roughness, metallicTexture},
        };
        const retro::renderer::material_specification materialSpecification = {
            textures,
            glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
            0.3f,
            0.4f,
        };
        m_Material = retro::renderer::material::create(
            materialSpecification);


        m_Scene = retro::scene::create("Test Scene");
        auto cerberus = m_Scene->create_actor();
        cerberus->add_component<retro::name_component>("Cerberus");
        cerberus->add_component<retro::model_renderer_component>(m_Model);
        cerberus->add_component<retro::material_component>(m_Material);
        auto transform = cerberus->add_component<retro::transform_component>();
        transform.scale = glm::vec3(0.05f);
        transform.rotation = glm::vec3(-1.5f, -2.1f, 0.0f);
        transform.position = glm::vec3(1.2f, 0.0f, 5.8f);

        
        retro::renderer::scene_renderer::set_scene(m_Scene);
        retro::renderer::scene_renderer::initialize();
    }

    void on_layer_registered() override
    {
    }

    void on_layer_unregistered() override
    {
    }

    void on_layer_updated() override
    {
        retro::renderer::scene_renderer::begin_render();

        retro::renderer::scene_renderer::end_render();

        const auto view = m_Scene->get_actor_registry().group<retro::model_renderer_component>
            (entt::get<retro::name_component, retro::transform_component>);
        ImGui::Begin("Actors");
        if (ImGui::TreeNode("List"))
        {
            for (auto&& [actor,model_renderer, name, transform] : view.each())
            {
                if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(actor)), "Asset %s",
                                    name.name.c_str()))
                {
                    ImGui::Text("Name %s", name.name);

                    {
                        ImGui::TextUnformatted("Scale");
                        ImGui::NextColumn();
                        ImGui::PushItemWidth(-1);
                        const std::string id = "##scale";
                        if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.scale), 0.1f));
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                    }

                    {
                        ImGui::TextUnformatted("Position");
                        ImGui::NextColumn();
                        ImGui::PushItemWidth(-1);
                        const std::string id = "##Position";
                        if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.position), 0.1f));
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                    }

                    {
                        ImGui::TextUnformatted("Rotation");
                        ImGui::NextColumn();
                        ImGui::PushItemWidth(-1);
                        const std::string id = "##Rotation";
                        if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.rotation), 0.1f));
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                    }
                    
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        /*
        // Render light model
        {
            auto lightMat = glm::mat4(1.0f);
            lightMat = translate(lightMat, m_Light->get_position());
            lightMat = scale(lightMat, {0.25f, 0.25f, 0.25f});
            m_Shader->set_mat4("uTransform", lightMat);
            m_Shader->set_int("material.hasAlbedoMap", 0);
            m_Shader->set_vec_float4("material.albedo", glm::vec4(m_Light->get_color(), 1.0f));
            retro::renderer::renderer::submit_command({
                m_Shader, m_LightModel->get_model_renderables()[0]->get_vertex_array_buffer(), nullptr, lightMat
            });
        }
        

        ImGui::Begin("Edit");
        ImGui::SliderFloat3("Scale", value_ptr(m_Scale), 0.02f, 5.0f);
        ImGui::SliderFloat3("Location", value_ptr(m_Translate), -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", value_ptr(m_Rotation), -5.0f, 5.0f);
        ImGui::SliderFloat3("Camera Pos", value_ptr(m_CameraLocation), -10.0f, 10.0f);
        ImGui::SliderFloat("Camera FOV", &m_CameraFov, 1.0f, 90.0f);
        if (ImGui::SliderFloat3("Light Position", value_ptr(m_LightPos), -5.0f, 5.0f))
        {
            m_Light->set_position(m_LightPos);
        }
        if (ImGui::ColorEdit3("Light Color", value_ptr(m_LightColor)))
        {
            m_Light->set_color(m_LightColor);
        }
        ImGui::End();

        ImGui::Begin("Assets");

        if (ImGui::TreeNode("List"))
        {
            for (auto& assetsCategory : retro::retro_application::get_application().get_assets_manager()->get_assets())
            {
                if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(assetsCategory.first)), "Asset %s",
                                    retro::asset::get_asset_to_string(assetsCategory.first).c_str()))
                {
                    for (const auto& key : assetsCategory.second | std::views::keys)
                    {
                        ImGui::Text("UUID: %lld", key->get());
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::Begin("Actors");

        if (ImGui::TreeNode("List"))
        {
            int i = 0;
            for (auto& actor : m_Scene->get_actor_registry().view<retro::name_component>())
            {
                if (ImGui::TreeNode(reinterpret_cast<void*>(i), "Actor %d", i))
                {
                    ImGui::Text(m_Scene->get_actor_registry().get<retro::name_component>(actor).name.c_str());
                    ImGui::TreePop();
                }
                i++;
            }
            ImGui::TreePop();
        }
        ImGui::End();

        */
        ImGui::Begin("Viewport");
        const auto viewportMinRegin = ImGui::GetWindowContentRegionMin();
        const auto viewportMaxRegin = ImGui::GetWindowContentRegionMax();
        const auto viewportOffset = ImGui::GetWindowPos();

        // Viewport bounds
        m_ViewportBounds[0] = {viewportMinRegin.x + viewportOffset.x, viewportMinRegin.y + viewportOffset.y};
        m_ViewportBounds[1] = {viewportMaxRegin.x + viewportOffset.x, viewportMaxRegin.y + viewportOffset.y};

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

        // Draw viewport
        ImGui::Image(
            reinterpret_cast<ImTextureID>(retro::renderer::scene_renderer::get_final_texture()),
            ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});

        ImGui::End();
    }

private:
    glm::vec3 m_Scale = glm::vec3(1.0f);
    glm::vec3 m_LightPos = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 m_LightColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 m_Rotation = glm::vec3(1.0f);
    glm::vec3 m_Translate = glm::vec3(1.0f);
    glm::vec2 m_ViewportBounds[2];
    glm::vec2 m_ViewportSize = {1920.0f, 1080.0f};
    retro::shared<retro::scene> m_Scene;
    retro::shared<retro::renderer::material> m_Material;
    retro::shared<retro::renderer::model> m_Model;
    retro::shared<retro::renderer::model> m_LightModel;
};

class sandbox_interface_layer : public retro::interface_layer
{
public:
    sandbox_interface_layer() : interface_layer("SandboxInterfaceLayer")
    {
        m_use_vsync = retro::retro_application::get_application().get_window()->is_vsync_enabled();
    }

    void on_interface_renderer() override
    {
        ImGui::ShowDemoWindow();
        ImGui::Begin("Sandbox");
        const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
        ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Renderer");
        if (ImGui::Button("Normal"))
        {
            retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::normal);
        }
        if (ImGui::Button("Wireframe"))
        {
            retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::wireframe);
        }
        if (ImGui::Button("Point"))
        {
            retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::point);
        }
        if (ImGui::Checkbox("VSync", &m_use_vsync))
        {
            retro::retro_application::get_application().get_window()->set_vsync_enabled(m_use_vsync);
        }
        ImGui::End();
    }

    void on_layer_registered() override
    {
    }

    void on_layer_unregistered() override
    {
    }

    void on_layer_updated() override
    {
    }

private:
    bool m_use_vsync;
};

class sandbox_application : public retro::retro_application
{
public:
    sandbox_application() : retro_application({"Sandbox"})
    {
        get_layers_manager()->register_layer(retro::create_shared<sandbox_layer>());
        get_interfaces_layer_manager()->register_layer(retro::create_shared<sandbox_interface_layer>());
    }

    ~sandbox_application() override
    {
    };
};

retro::retro_application* retro::create_retro_application()
{
    return new sandbox_application();
}
