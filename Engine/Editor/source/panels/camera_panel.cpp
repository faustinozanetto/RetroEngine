#include "camera_panel.h"

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
    camera_panel::camera_panel()
    {
    }

    camera_panel::~camera_panel()
    {
    }

    void camera_panel::on_render_panel()
    {
        const auto& camera = renderer::scene_renderer::get_camera();
        ImGui::Begin("Camera");
        if (editor_interface_utils::draw_property("Field of View", m_camera_fov, 1.0f, 120.0f, 0.1f))
        {
            camera->set_fov(m_camera_fov);
        }
        if (editor_interface_utils::draw_property("Near Plane", m_near_plane, 0.001f, 5000.0f, 1.0f))
        {
            camera->set_near_plane(m_near_plane);
        }
        if (editor_interface_utils::draw_property("Far Plane", m_far_plane, 1.0f, 5000.0f, 1.0f))
        {
            camera->set_far_plane(m_far_plane);
        }
        ImGui::End();
    }

    void camera_panel::on_update_panel(float time)
    {
    }
}
