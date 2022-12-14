#include "pch.h"

#include "actor.h"
#include "scene.h"

namespace retro
{
  scene::scene() : scene("Scene")
  {
  }

  scene::scene(const std::string &scene_name)
  {
    logger::line();
    actor::s_scene = this;
    m_scene_name = scene_name;
    logger::info("Creating scene with name: " + scene_name);
  }

  scene::~scene()
  {
  }

  shared<actor> scene::create_actor()
  {
    const entt::entity handle = m_actors_registry.create();
    shared<actor> actor = actor::create(handle);
    m_actors.push_back(actor);
    return actor;
  }

  shared<scene> scene::create(const std::string &scene_name)
  {
    return create_shared<scene>(scene_name);
  }
}