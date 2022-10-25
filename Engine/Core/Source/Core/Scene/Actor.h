#pragma once

#include "entt/entt.hpp"

namespace Retro
{
  class Actor
  {
  public:
    Actor();
    ~Actor() = default;

    entt::entity GetActorHandle() const { return m_ActorHandle; };

    operator bool() const
    {
      return m_ActorHandle != entt::null;
    }

  private:
    entt::entity m_ActorHandle{entt::null};
  };
}