#include "pch.h"

#include "asset.h"
#include "core/application/retro_application.h"

namespace retro
{
	asset::asset()
	{
		m_uuid = create_shared<uuid>();
		m_asset_type = asset_type::none;
		retro_application::get_application().get_assets_manager()->register_asset(shared<asset>(this));
	}

	asset::asset(asset_type asset_type)
	{
		m_uuid = create_shared<uuid>();
		m_asset_type = asset_type;
		retro_application::get_application().get_assets_manager()->register_asset(shared<asset>(this));
	}

	const shared<uuid> &asset::get_uuid() const
	{
		return m_uuid;
	}

	std::string asset::get_asset_to_string(asset_type asset_type)
	{
		switch (asset_type)
		{
		case asset_type::none:
			return "None";
		case asset_type::model:
			return "Model";
		case asset_type::texture:
			return "Texture";
		case asset_type::shader:
			return "Shader";
		}
		return "";
	}

	shared<asset> asset::create(asset_type asset_type)
	{
		return create_shared<asset>(asset_type);
	}
}
