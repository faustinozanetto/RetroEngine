#include "pch.h"

#include "actor.h"

namespace Retro
{
	actor::actor()
	{
	}

	actor::actor(entt::entity handle)
	{
		m_ActorHandle = handle;
	}

	actor::~actor()
	{
	}

	shared<actor> actor::create(entt::entity handle)
	{
		return create_shared<actor>(handle);
	}
}
