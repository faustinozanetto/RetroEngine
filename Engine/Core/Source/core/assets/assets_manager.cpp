#include "pch.h"

#include "assets_manager.h"

#include "core/assert.h"

namespace retro
{
    assets_manager::assets_manager() : manager("AssetsManager")
    {
    }

    assets_manager::~assets_manager()
    {
    }

    void assets_manager::shutdown()
    {
    }

    shared<renderer::texture> assets_manager::create_texture(
        const renderer::texture_specification& texture_specification)
    {
        auto texture = renderer::texture::create(texture_specification);
        m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
        RETRO_CORE_ASSERT(texture, "Failed to create texture.");

        return texture;
    }

    shared<renderer::material> assets_manager::create_material(
        const renderer::material_specification& material_specification)
    {
        auto material = renderer::material::create(material_specification);
        m_assets[asset_type::material].insert(std::make_pair(material->get_uuid(), material));
        RETRO_CORE_ASSERT(material, "Failed to create material.");

        return material;
    }

    shared<renderer::model> assets_manager::create_model(const renderer::model_specification& model_specification)
    {
        auto model = renderer::model::create(model_specification);
        m_assets[asset_type::model].insert(std::make_pair(model->get_uuid(), model));
        RETRO_CORE_ASSERT(model, "Failed to create model.");

        return model;
    }

    unique<assets_manager> assets_manager::create()
    {
        return create_unique<assets_manager>();
    }
}
