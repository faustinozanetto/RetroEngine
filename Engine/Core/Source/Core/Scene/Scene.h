#pragma once

#include "entt/entt.hpp"
#include "actor.h"

namespace retro
{
  class scene
  {
  public:
    scene();
    scene(const std::string &scene_name);
    ~scene();

    shared<actor> create_actor();

    const entt::registry &get_actor_registry() { return m_actors_registry; }

    static shared<scene> create(const std::string &scene_name);

  private:
    entt::registry m_actors_registry;
    std::string m_scene_name;

    std::vector<shared<actor>> m_actors;
  };
}