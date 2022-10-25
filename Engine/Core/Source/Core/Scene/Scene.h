#pragma once

#include "entt/entt.hpp"

namespace Retro
{
  class Scene
  {
  public:
    Scene();
    ~Scene();

  private:
    entt::registry m_ActorsRegistry;
  };
}