#include "pch.h"
#include "Material.h"

namespace Retro::Renderer
{
    Material::Material()
    {
        m_MaterialSpecification = FMaterialSpecification();
    }

    Material::Material(const FMaterialSpecification& materialSpecification)
    {
        m_MaterialSpecification = materialSpecification;
    }

    Material::~Material()
    {
    }

    void Material::Bind()
    {
        if (!m_MaterialSpecification.shader) return;
        // Bind each texture in the material.
        for (auto const& [type, texture] : m_MaterialSpecification.textures)
        {
            const std::string& uniformName = GetTextureUniformEnabledValue(type);
            m_MaterialSpecification.shader->SetInt("material.hasAlbedoMap", texture.enabled ? 1 : 0);
            if (texture.enabled) texture.texture->Bind(0);
        }
        // Bind colors and parameters.
        m_MaterialSpecification.shader->SetVecFloat4("material.albedo", m_MaterialSpecification.albedo);
    }

    void Material::UnBind()
    {
    }

    void Material::SetShader(const Ref<Shader>& shader)
    {
        m_MaterialSpecification.shader = shader;
    }

    const FMaterialTexture& Material::GetMaterialTexture(EMaterialTextureType type)
    {
        return m_MaterialSpecification.textures.find(type)->second;
    }

    Ref<Material> Material::Create()
    {
        return CreateRef<Material>();
    }

    Ref<Material> Material::Create(const FMaterialSpecification& materialSpecification)
    {
        return CreateRef<Material>(materialSpecification);
    }

    const std::string& Material::GetTextureUniformEnabledValue(EMaterialTextureType type)
    {
        switch (type)
        {
        case EMaterialTextureType::Albedo: return "material.hasAlbedoMap";
        }
        return "";
    }
}
