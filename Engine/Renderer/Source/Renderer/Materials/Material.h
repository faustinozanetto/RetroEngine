#pragma once
#include <map>

#include "Core/Base.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Textures/Texture.h"

namespace Retro::Renderer
{
	enum class EMaterialTextureType
	{
		Albedo = 0,
		Normal = 1,
		Roughness = 2,
		Metallic = 3
	};

	struct FMaterialTexture
	{
		Ref<Texture> texture;
		bool enabled;

		FMaterialTexture(const Ref<Texture>& texture, bool enabled) : texture(texture), enabled(enabled)
		{
		}
	};

	struct FMaterialSpecification
	{
		Ref<Shader> shader;
		std::map<EMaterialTextureType, FMaterialTexture> textures;
		glm::vec4 albedo = glm::vec4(1.0f);
		float metallic = 0.0f;
		float roughness = 1.0f;

		FMaterialSpecification() = default;

		FMaterialSpecification(const Ref<Shader>& shader,
		                       const std::map<EMaterialTextureType, FMaterialTexture>& textures,
		                       const glm::vec4& albedo, float metallic, float roughness) : shader(shader),
			textures(textures), albedo(albedo),
			metallic(metallic), roughness(roughness)
		{
		}
	};

	class Material : public GraphicsObject
	{
	public:
		/* Constructor & Destructor */
		Material();
		Material(const FMaterialSpecification& materialSpecification);
		~Material() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		void SetShader(const Ref<Shader>& shader);

		const FMaterialTexture& GetMaterialTexture(EMaterialTextureType type);
		const FMaterialSpecification& GetMaterialSpecification() const { return m_MaterialSpecification; }

		void SetRoughness(float roughness) { m_MaterialSpecification.roughness = roughness; }
		void SetMetallic(float metallic) { m_MaterialSpecification.metallic = metallic; }

		/* Instantiate */
		static Ref<Material> Create();
		static Ref<Material> Create(const FMaterialSpecification& materialSpecification);

	private:
		const std::string GetTextureUniformEnabledValue(EMaterialTextureType type);
		uint32_t GetMaterialTextureBindSlot(EMaterialTextureType type);

	private:
		FMaterialSpecification m_MaterialSpecification;
	};
}
