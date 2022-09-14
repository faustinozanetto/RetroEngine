#include "pch.h"
#include "Manager.h"

namespace Retro
{
    Manager::Manager(const std::string& managerName)
    {
        m_ManagerName = managerName;
        Logger::Info("Manager::Manager | Creating manager: " + managerName);
    }

    const std::string& Manager::GetManagerName() const
    {
        return m_ManagerName;
    }
}
