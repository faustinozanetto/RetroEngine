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
    };

    struct FMaterialTexture
    {
        EMaterialTextureType type;
        Ref<Texture> texture;
        bool enabled;

        FMaterialTexture(EMaterialTextureType type, const Ref<Texture> &texture, bool enabled) : type(type),
                                                                                                 texture(texture), enabled(enabled)
        {
        }
    };

    struct FMaterialSpecification
    {
        Ref<Shader> shader;
        std::map<EMaterialTextureType, FMaterialTexture> textures;
        glm::vec4 albedo;

        FMaterialSpecification() : textures({}), albedo(1.0f)
        {
        }

        FMaterialSpecification(const Ref<Shader> &shader,
                               const std::map<EMaterialTextureType, FMaterialTexture> &textures,
                               const glm::vec4 &albedo) : shader(shader), textures(textures), albedo(albedo)
        {
        }
    };

    class Material : public GraphicsObject
    {
    public:
        /* Constructor & Destructor */
        Material();
        Material(const FMaterialSpecification &materialSpecification);
        ~Material();

        /* Methods */
        void Bind() override;
        void UnBind() override;

        void SetShader(const Ref<Shader> &shader);

        const FMaterialTexture &GetMaterialTexture(EMaterialTextureType type);

        /* Instantiate */
        static Ref<Material> Create();
        static Ref<Material> Create(const FMaterialSpecification &materialSpecification);

    private:
        const std::string &GetTextureUniformEnabledValue(EMaterialTextureType type);
        uint32_t GetMaterialTextureBindSlot(EMaterialTextureType type);

    private:
        FMaterialSpecification m_MaterialSpecification;
    };
}
