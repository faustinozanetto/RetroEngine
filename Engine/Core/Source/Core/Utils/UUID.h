#pragma once

namespace Retro {
	class UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		const uint64_t Get() { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}