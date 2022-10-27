#include "pch.h"

#include "actor.h"

namespace retro
{
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
