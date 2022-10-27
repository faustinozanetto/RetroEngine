#pragma once

namespace Retro
{
	class uuid
	{
	public:
		uuid();
		uuid(uint64_t uuid);
		uuid(const uuid &) = default;

		const uint64_t get() { return m_uuid; }

	private:
		uint64_t m_uuid;
	};
}
