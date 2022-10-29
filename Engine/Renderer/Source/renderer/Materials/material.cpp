#include "pch.h"

#include "material.h"

namespace retro::renderer
{
    material::material()
    {
        m_material_specification = material_specification();
    }

    material::material(const material_specification& material_specification)
    {
        m_material_specification = material_specification;
    }

    material::~material()
    {
    }

    void material::bind()
    {
        if (!m_material_specification.mat_shader) return;
        // Bind colors and parameters.
        m_material_specification.mat_shader->set_vec_float4("material.albedo", m_material_specification.albedo);
        m_material_specification.mat_shader->set_float("material.metallic", m_material_specification.metallic);
        m_material_specification.mat_shader->set_float("material.roughness", m_material_specification.roughness);
        // Bind each texture in the material.
        for (const auto& [type, texture] : m_material_specification.textures)
        {
            const std::string& uniformName = get_material_texture_uniform(type);
            m_material_specification.mat_shader->set_int(uniformName, texture.enabled ? 1 : 0);
            if (texture.enabled) texture.mat_texture->bind(get_material_texture_bind_slot(type));
        }
    }

    void material::un_bind()
    {
        // UnBind each texture in the material.
        /*
         for (auto const& [type, texture] : m_MaterialSpecification.textures)
         {
             if (texture.enabled && texture.texture) texture.texture->UnBind();
         }
         */
    }

    void material::set_shader(const shared<shader>& shader)
    {
        m_material_specification.mat_shader = shader;
    }

    const material_texture& material::get_material_texture(material_texture_type material_texture_type)
    {
        return m_material_specification.textures.find(material_texture_type)->second;
    }

    shared<material> material::create()
    {
        return create_shared<material>();
    }

    shared<material> material::create(const material_specification& material_specification)
    {
        return create_shared<material>(material_specification);
    }

    const std::string material::get_material_texture_uniform(material_texture_type material_texture_type)
    {
        switch (material_texture_type)
        {
        case material_texture_type::albedo: return "material.hasAlbedoMap";
        case material_texture_type::normal: return "material.hasNormalMap";
        case material_texture_type::roughness: return "material.hasRoughnessMap";
        case material_texture_type::metallic: return "material.hasMetallicMap";
        case material_texture_type::ambient_occlusion: return "material.hasAmbientOcclusionMap";
        }
        return "";
    }

    uint32_t material::get_material_texture_bind_slot(material_texture_type material_texture_type)
    {
        switch (material_texture_type)
        {
        case material_texture_type::albedo: return 0;
        case material_texture_type::normal: return 1;
        case material_texture_type::roughness: return 2;
        case material_texture_type::metallic: return 3;
        case material_texture_type::ambient_occlusion: return 4;
        }
        return 0;
    }
}
