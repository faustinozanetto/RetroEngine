#include "pch.h"

#include "assets_mangaer.h"

namespace Retro
{
	assets_mangaer::assets_mangaer() : manager("AssetssManager")
	{
	}

	assets_mangaer::~assets_mangaer()
	{
	}

	void assets_mangaer::Shutdown()
	{
		m_assets.clear();
	}

	void assets_mangaer::RegisterAsset(const shared<asset> asset)
	{
		m_assets[asset->get_asset_type()].insert(std::make_pair(asset->get_uuid(), asset));
	}

	unique<assets_mangaer> assets_mangaer::Create()
	{
		return create_unique<assets_mangaer>();
	}
}
