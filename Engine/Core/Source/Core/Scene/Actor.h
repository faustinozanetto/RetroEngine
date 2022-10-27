#pragma once

#include <entt/entt.hpp>

namespace Retro
{
	class actor
	{
	public:
		actor();
		actor(entt::entity handle);
		~actor();

		entt::entity get_actor_handle() const { return m_actor_handle; };

		operator bool() const
		{
			return m_actor_handle != entt::null;
		}

		static shared<actor> create(entt::entity handle);

	private:
		entt::entity m_actor_handle{entt::null};
	};
}
