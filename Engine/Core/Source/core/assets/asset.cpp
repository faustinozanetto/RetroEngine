#include "pch.h"

#include "asset.h"

namespace retro
{
    asset::asset()
    {
        m_uuid = create_shared<uuid>();
        m_asset_type = asset_type::none;
    }

    asset::asset(asset_type asset_type)
    {
        m_uuid = create_shared<uuid>();
        m_asset_type = asset_type;
    }

    shared<uuid>& asset::get_uuid()
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
        case asset_type::material:
            return "Material";
        default: return "Unknown";
        }
    }
}
