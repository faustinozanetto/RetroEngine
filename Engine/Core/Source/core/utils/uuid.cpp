#include "pch.h"

#include "uuid.h"

namespace retro
{
	static std::random_device s_random_device;
	static std::mt19937_64 s_engine(s_random_device());
	static std::uniform_int_distribution<uint64_t> s_uniform_distribution;

	uuid::uuid()
			: m_uuid(s_uniform_distribution(s_engine))
	{
	}

	uuid::uuid(uint64_t uuid)
			: m_uuid(uuid)
	{
	}
}
