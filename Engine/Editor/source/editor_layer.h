#pragma once

#include "core/base.h"
#include "core/layers/layer.h"
#include "core/scene/scene.h"
#include "renderer/camera/camera.h"

namespace retro::editor
{
    class editor_layer : public layer
    {
    public:
        explicit editor_layer(const std::string& layer_name);
        ~editor_layer() override;

        void on_layer_registered() override;
        void on_layer_unregistered() override;
        void on_layer_updated() override;
        std::pair<float, float> PanSpeed() const;

    private:
        void initialize_editor_scene();
        
        shared<renderer::camera> m_camera;
    };
}
