#pragma once

namespace retro
{
	class manager
	{
	public:
		manager(const std::string& manager_name);
		virtual ~manager() = default;

		virtual void shutdown() = 0;

		/* Methods */
		const std::string& get_manager_name() const;
	private:
		std::string m_manager_name;
	};
}
