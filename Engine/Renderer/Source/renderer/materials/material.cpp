#include "pch.h"

#include "material.h"

namespace retro::renderer
{
    material::material() : asset(asset_type::material)
    {
        m_material_specification = material_specification();
    }

    material::material(const material_specification& material_specification): asset(asset_type::material)
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
        m_material_specification.mat_shader->set_float("material.ambient_occlusion",
                                                       m_material_specification.ambient_occlusion);
        // Bind each texture in the material.
        for (const auto& [type, texture] : m_material_specification.textures)
        {
            const std::string& uniformName = get_material_texture_uniform(type);
            m_material_specification.mat_shader->set_int(uniformName, texture.enabled ? 1 : 0);
            if (texture.mat_texture && texture.enabled) texture.mat_texture->bind(static_cast<int>(get_material_texture_bind_slot(type)));
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

    void material::set_texture_enabled(material_texture_type type, bool enabled)
    {
        m_material_specification.textures[type].enabled = enabled;
    }

    const material_texture& material::get_material_texture(material_texture_type material_texture_type)
    {
        return m_material_specification.textures.find(material_texture_type)->second;
    }

    std::string material::get_texture_type_to_string(material_texture_type material_texture_type)
    {
        switch (material_texture_type)
        {
        case material_texture_type::albedo: return "Albedo";
        case material_texture_type::normal: return "Normal";
        case material_texture_type::roughness: return "Roughness";
        case material_texture_type::metallic: return "Metallic";
        case material_texture_type::ambient_occlusion: return "Ambient Occlusion";
        }
        return "";
    }

    void material::serialize()
    {
        std::fstream material_file;
        material_file.open("Assets/Serialized/Materials/mat.rmat",
                           std::fstream::in | std::fstream::out | std::fstream::trunc);
        material_file << "material {\n";
        material_file << "  uuid: " + std::to_string(get_uuid().get()->get()) + "\n";
        material_file << "  parameters {\n";
        material_file << "      albedo: (" + std::to_string(m_material_specification.albedo.r) + ", " + std::to_string(
                m_material_specification.albedo.
                                         g) + ", " + std::to_string(m_material_specification.albedo.b) + ", " +
            std::to_string(m_material_specification.albedo.a) + ")\n";
        material_file << "      roughness: " + std::to_string(m_material_specification.roughness) + "\n";
        material_file << "      metallic: " + std::to_string(m_material_specification.metallic) + "\n";
        material_file << "      ambient_occlusion: " + std::to_string(m_material_specification.ambient_occlusion) + "\n";
        material_file << "  }\n";
        material_file << "  textures {\n";
        for (auto& mat_text : m_material_specification.textures)
        {
            material_file << "      " + get_texture_type_to_string(mat_text.first) + " {\n";
            material_file << "          enabled: " + std::to_string(mat_text.second.enabled) + "\n";
            material_file << "      }\n";
        }
        material_file << "  }\n";
        material_file << "}\n";
        material_file.close();
    }

    shared<material> material::create()
    {
        return create_shared<material>();
    }

    shared<material> material::create(const material_specification& material_specification)
    {
        return create_shared<material>(material_specification);
    }

    std::string material::get_material_texture_uniform(material_texture_type material_texture_type)
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
