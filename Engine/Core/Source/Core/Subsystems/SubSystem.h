#pragma once

namespace Retro
{
	class SubSystem
	{
	public:
		/* Constructor & Destructor */
		SubSystem(const std::string& subSystemName);
		virtual ~SubSystem() = default;

		/* Methods */
		virtual void OnSubSystemStart() = 0;
		virtual void OnSubSystemStop() = 0;
		const std::string& GetSubSystemName() const;

		/* Instantiate */
	private:
		std::string m_SubSystemName;
	};
}
