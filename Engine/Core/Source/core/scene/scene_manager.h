#pragma once
#include "scene.h"
#include "core/managers/manager.h"

namespace retro
{
    class scene_manager : public manager
    {
    public:
        /* Constructor & Destructor */
        scene_manager(const std::string& manager_name);
        ~scene_manager() override;

        /* Methods */
        void shutdown() override;

        void set_active_scene(const shared<scene>& scene) { m_active_scene = scene; }
        shared<scene>& get_active_scene() { return m_active_scene; }

        /* Instantiate */
        static unique<scene_manager> create(const std::string& manager_name);
    
    private:
        shared<scene> m_active_scene;
    };
}
