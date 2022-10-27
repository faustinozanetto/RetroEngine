#include "pch.h"
#include "manager.h"

namespace Retro
{
	manager::manager(const std::string &manager_name)
	{
		m_manager_name = manager_name;
		Logger::Line();
		Logger::Info("Manager::Manager | Creating manager: " + managerName);
		Logger::Line();
	}

	const std::string &manager::get_manager_name() const
	{
		return m_manager_name;
	}
}
