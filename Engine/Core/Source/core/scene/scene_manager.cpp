#include "pch.h"
#include "scene_manager.h"

namespace retro
{
    scene_manager::scene_manager(const std::string& manager_name) : manager(manager_name)
    {
    }

    scene_manager::~scene_manager()
    {
    }

    void scene_manager::shutdown()
    {
    }

    unique<scene_manager> scene_manager::create(const std::string& manager_name)
    {
        return create_unique<scene_manager>(manager_name);
    }
}
