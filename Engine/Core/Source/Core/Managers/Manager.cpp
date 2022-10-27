#include "pch.h"

#include "manager.h"

namespace retro
{
	manager::manager(const std::string &manager_name)
	{
		m_manager_name = manager_name;
		logger::line();
		logger::info("manager::manager | Creating manager: " + manager_name);
		logger::line();
	}

	const std::string &manager::get_manager_name() const
	{
		return m_manager_name;
	}
}
