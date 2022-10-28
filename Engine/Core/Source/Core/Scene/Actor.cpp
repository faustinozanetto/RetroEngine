#include "pch.h"

#include "actor.h"

namespace retro
{
	scene* actor::s_scene = nullptr;

	actor::actor()
	{
	}

	actor::actor(entt::entity handle)
	{
		m_actor_handle = handle;
	}

	actor::~actor()
	{
	}

	shared<actor> actor::create(entt::entity handle)
	{
		return create_shared<actor>(handle);
	}
}
