#pragma once

namespace Retro
{
    class Manager
    {
    public:
        Manager(const std::string& managerName);
        virtual ~Manager() = default;

        virtual void Shutdown() = 0;

        /* Methods */
        const std::string& GetManagerName() const;
    private:
        std::string m_ManagerName;
    };
}