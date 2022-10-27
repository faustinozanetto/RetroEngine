#pragma once

namespace retro
{
	class sub_system
	{
	public:
		/* Constructor & Destructor */
		sub_system(const std::string &
									 sub_system_name);
		virtual ~sub_system() = default;

		/* Methods */
		virtual void on_sub_system_start() = 0;
		virtual void on_sub_system_stop() = 0;
		const std::string &get_sub_system_name() const;

		/* Instantiate */
	private:
		std::string m_sub_system_name;
	};
}
