#pragma once

#include "core/managers/manager.h"
#include "core/assets/asset.h"

namespace retro
{
	class assets_manager : public manager
	{
	public:
		/* Constructor & Destructor */
		assets_manager();
		~assets_manager() override;

		/* Methods */
		void shutdown() override;
		void register_asset(const shared<asset>& asset);

		const std::unordered_map<asset_type, std::unordered_map<shared<uuid>, shared<asset>>> &get_assets() const
		{
			return m_assets;
		}

		/* Instantiate */
		static unique<assets_manager> create();

	private:
		std::unordered_map<asset_type, std::unordered_map<shared<uuid>, shared<asset>>> m_assets;
	};
}
