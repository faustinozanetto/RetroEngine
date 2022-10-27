#pragma once

#include "core/utils/uuid.h"
#include "Core/Base.h"

namespace Retro
{
	enum class asset_type
	{
		none = 0,
		model = 1,
		texture = 2,
		shader = 3,
	};

	class asset
	{
	public:
		asset();
		asset(asset_type asset_type);
		virtual ~asset() = default;

		const shared<uuid> &get_uuid() const;
		const asset_type get_asset_type() { return m_asset_type; }

		static std::string get_asset_to_string(asset_type asset_type);
		static shared<asset> create(asset_type asset_type);

	private:
		shared<uuid> m_uuid;
		asset_type m_asset_type;
	};
}
