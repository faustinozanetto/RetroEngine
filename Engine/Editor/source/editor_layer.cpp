#include "editor_layer.h"

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
        renderer::scene_renderer::set_scene(m_scene);
        renderer::scene_renderer::initialize(m_camera);
    }

    void editor_layer::on_layer_unregistered()
    {
    }

    void editor_layer::on_layer_updated()
    {
    }

    void editor_layer::initialize_editor_scene()
    {
        m_scene = scene::create("Editor Scene");
        m_camera = retro::create_shared<renderer::camera>(50.0f, 0.01f, 2000.0f);
    }
}
