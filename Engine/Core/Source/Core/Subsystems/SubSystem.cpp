#include "pch.h"
#include "SubSystem.h"

namespace Retro
{
    SubSystem::SubSystem(const std::string& subSystemName)
    {
        m_SubSystemName = subSystemName;
    }

    const std::string& SubSystem::GetSubSystemName() const
    {
        return m_SubSystemName;
    }
}
