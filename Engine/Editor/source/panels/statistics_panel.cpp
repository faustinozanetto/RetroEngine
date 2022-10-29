#include "statistics_panel.h"

#include "imgui.h"

namespace retro::editor
{
    statistics_panel::statistics_panel()
    {
    }

    statistics_panel::~statistics_panel()
    {
    }

    void statistics_panel::on_update_panel(float time)
    {
    }

    void statistics_panel::on_render_panel()
    {
        ImGui::Begin("Statistics");
        const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
        ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}
