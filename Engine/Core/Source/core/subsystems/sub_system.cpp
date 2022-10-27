#include "pch.h"

#include "sub_system.h"

namespace retro
{
	sub_system::sub_system(const std::string &sub_system_name)
	{
		m_sub_system_name = sub_system_name;
	}

	const std::string &sub_system::get_sub_system_name() const
	{
		return m_sub_system_name;
	}
}
