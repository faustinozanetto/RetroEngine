#include "renderer_panel.h"

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
    renderer_panel::renderer_panel()
    {
        m_use_vsync = retro_application::get_application().get_window()->get_window_specification().v_sync;
    }

    renderer_panel::~renderer_panel()
    {
    }

    void renderer_panel::on_render_panel()
    {
        ImGui::Begin("Renderer");
        if (editor_interface_utils::draw_property("Enable VSync", m_use_vsync))
        {
            retro_application::get_application().get_window()->set_vsync_enabled(m_use_vsync);
        }
        ImGui::Separator();
        if (ImGui::TreeNode("Frame Buffers"))
        {
            if (ImGui::TreeNode("GeometryBuffer"))
            {
                for (const auto& attachment : renderer::scene_renderer::get_geometry_frame_buffer()->
                     get_color_attachments_specifications())
                {
                    if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment.first)), "Buffer %s",
                                        attachment.second.name.c_str()))
                    {
                        ImGui::Columns(2);

                        if (ImGui::ImageButton(
                            reinterpret_cast<ImTextureID>(attachment.first),
                            {64.0f, 64.0f},
                            ImVec2(0.0f, 1.0f),
                            ImVec2(1.0f, 0.0f)))
                        {
                            editor_main_interface::s_render_target = attachment.first;
                        }
                        // Hover tooltip.
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::Image(reinterpret_cast<ImTextureID>(attachment.first), ImVec2(256, 256),
                                         ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                            ImGui::EndTooltip();
                        }
                        ImGui::NextColumn();

                        ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_geometry_frame_buffer()->get_width(),
                                    renderer::scene_renderer::get_geometry_frame_buffer()->get_height());
                        ImGui::NextColumn();
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("FinalBuffer"))
            {
                for (const auto& attachment : renderer::scene_renderer::get_final_frame_buffer()->
                     get_color_attachments_specifications())
                {
                    if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment.first)), "Buffer %s",
                                        attachment.second.name.c_str()))
                    {
                        ImGui::Columns(2);

                        if (ImGui::ImageButton(
                            reinterpret_cast<ImTextureID>(attachment.first),
                            {64.0f, 64.0f},
                            ImVec2(0.0f, 1.0f),
                            ImVec2(1.0f, 0.0f)))
                        {
                            editor_main_interface::s_render_target = attachment.first;
                        }
                        // Hover tooltip.
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::Image(reinterpret_cast<ImTextureID>(attachment.first), ImVec2(256, 256),
                                         ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                            ImGui::EndTooltip();
                        }
                        ImGui::NextColumn();

                        ImGui::Text("Size: %dx%d", renderer::scene_renderer::get_geometry_frame_buffer()->get_width(),
                                    renderer::scene_renderer::get_geometry_frame_buffer()->get_height());
                        ImGui::NextColumn();
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Environment"))
        {
            const std::string& capture_dimensions = std::format(
                "{}x{}",
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      capture_size,
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      capture_size);
            editor_interface_utils::draw_property("Capture Dimensions", capture_dimensions);
            const std::string& irradiance_size = std::format(
                "{}x{}",
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      irradiance_size,
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      irradiance_size);
            editor_interface_utils::draw_property("Irradiance Dimensions", irradiance_size);
            const std::string& prefilter_size = std::format(
                "{}x{}",
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      prefilter_size,
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      prefilter_size);
            editor_interface_utils::draw_property("Prefilter Dimensions", prefilter_size);
            const std::string& brdf_lut_size = std::format(
                "{}x{}",
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      brdf_lut_size,
                renderer::scene_renderer::get_lighting_environment()->get_lighting_environment_specification().
                                                                      brdf_lut_size);
            editor_interface_utils::draw_property("BRDF Lut Dimensions", brdf_lut_size);
            // HDRi
            if (ImGui::TreeNode("Environment Map"))
            {
                ImGui::Image(
                    reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                        get_hdri_texture()),
                    {64.0f, 64.0f},
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f)
                );

                // Hover tooltip.
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                                     get_hdri_texture()), ImVec2(256, 256),
                                 ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                    ImGui::EndTooltip();
                }
                ImGui::TreePop();
            }
            // BRDF Lut
            if (ImGui::TreeNode("Environment BRDF Lut"))
            {
                ImGui::Image(
                    reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                        get_brdf_lut_texture()),
                    {64.0f, 64.0f},
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f)
                );

                // Hover tooltip.
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                                     get_brdf_lut_texture()), ImVec2(256, 256),
                                 ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                    ImGui::EndTooltip();
                }
                ImGui::TreePop();
            }

            /*
            // Prefilter Map
            if (ImGui::TreeNode("Environment Prefilter Map"))
            {
                ImGui::Image(
                    reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                        get_prefilter_texture()),
                    {64.0f, 64.0f},
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f)
                );

                // Hover tooltip.
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                                     get_prefilter_texture()), ImVec2(256, 256),
                                 ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                    ImGui::EndTooltip();
                }
                ImGui::TreePop();
            }

            // Irradiance Map
            if (ImGui::TreeNode("Environment Irradiance Map"))
            {
                ImGui::Image(
                    reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                        get_irradiance_texture()),
                    {64.0f, 64.0f},
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f)
                );

                // Hover tooltip.
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Image(reinterpret_cast<ImTextureID>(renderer::scene_renderer::get_lighting_environment()->
                                     get_irradiance_texture()), ImVec2(256, 256),
                                 ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                    ImGui::EndTooltip();
                }
                ImGui::TreePop();
            }
            */
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void renderer_panel::on_update_panel(float time)
    {
    }
}
