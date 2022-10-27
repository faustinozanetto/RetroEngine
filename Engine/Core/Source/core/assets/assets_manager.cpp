#include "pch.h"

#include "assets_manager.h"

namespace retro
{
	assets_manager::assets_manager() : manager("AssetssManager")
	{
	}

	assets_manager::~assets_manager()
	{
	}

	void assets_manager::shutdown()
	{
		m_assets.clear();
	}

	void assets_manager::register_asset(const shared<asset> asset)
	{
		m_assets[asset->get_asset_type()].insert(std::make_pair(asset->get_uuid(), asset));
	}

	unique<assets_manager> assets_manager::create()
	{
		return create_unique<assets_manager>();
	}
}
