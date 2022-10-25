#include "pch.h"
#include "Manager.h"

namespace Retro
{
	Manager::Manager(const std::string& managerName)
	{
		m_ManagerName = managerName;
		Logger::Line();
		Logger::Info("Manager::Manager | Creating manager: " + managerName);
		Logger::Line();
	}

	const std::string& Manager::GetManagerName() const
	{
		return m_ManagerName;
	}
}
