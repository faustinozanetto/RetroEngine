#pragma once

#include "Core/Base.h"
#include "Renderer/Renderables/Renderable.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Retro::Renderer
{
    class Model
    {
    public:
        /* Constructor & Destructor */
        Model(const std::string& modelPath);
        ~Model();

        /* Instantiate */
        static Ref<Model> Create(const std::string& modelPath);

        /* Methods */
        const std::vector<Ref<Renderable>>& GetModelRenderables();
    private:
        bool LoadModelFromPath(const std::string& path);
        bool ProcessModelNode(const aiNode* node);
        Ref<Renderable> ParseRenderable(const aiMesh* mesh);

    private:
        const aiScene* m_AssimpScene{};
        std::vector<Ref<Renderable>> m_Renderables;
        std::string m_ModelPath;
    };
}
