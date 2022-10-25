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
		// Bind colors and parameters.
		m_MaterialSpecification.shader->SetVecFloat4("material.albedo", m_MaterialSpecification.albedo);
		m_MaterialSpecification.shader->SetFloat("material.metallic", m_MaterialSpecification.metallic);
		m_MaterialSpecification.shader->SetFloat("material.roughness", m_MaterialSpecification.roughness);
		// Bind each texture in the material.
		for (const auto& [type, texture] : m_MaterialSpecification.textures)
		{
			const std::string& uniformName = GetTextureUniformEnabledValue(type);
			m_MaterialSpecification.shader->SetInt(uniformName, texture.enabled ? 1 : 0);
			if (texture.enabled) texture.texture->Bind(GetMaterialTextureBindSlot(type));
		}
	}

	void Material::UnBind()
	{
		// UnBind each texture in the material.
		/*
		 for (auto const& [type, texture] : m_MaterialSpecification.textures)
		 {
		     if (texture.enabled && texture.texture) texture.texture->UnBind();
		 }
		 */
	}

	void Material::SetShader(const Shared<Shader>& shader)
	{
		m_MaterialSpecification.shader = shader;
	}

	const FMaterialTexture& Material::GetMaterialTexture(EMaterialTextureType type)
	{
		return m_MaterialSpecification.textures.find(type)->second;
	}

	Shared<Material> Material::Create()
	{
		return CreateShared<Material>();
	}

	Shared<Material> Material::Create(const FMaterialSpecification& materialSpecification)
	{
		return CreateShared<Material>(materialSpecification);
	}

	const std::string Material::GetTextureUniformEnabledValue(EMaterialTextureType type)
	{
		switch (type)
		{
		case EMaterialTextureType::Albedo: return "material.hasAlbedoMap";
		case EMaterialTextureType::Normal: return "material.hasNormalMap";
		case EMaterialTextureType::Roughness: return "material.hasRoughnessMap";
		case EMaterialTextureType::Metallic: return "material.hasMetallicMap";
		}
		return "";
	}

	uint32_t Material::GetMaterialTextureBindSlot(EMaterialTextureType type)
	{
		switch (type)
		{
		case EMaterialTextureType::Albedo: return 0;
		case EMaterialTextureType::Normal: return 1;
		case EMaterialTextureType::Roughness: return 2;
		case EMaterialTextureType::Metallic: return 3;
		}
		return 0;
	}
}
