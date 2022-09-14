#include "pch.h"
#include "VertexObjectBuffer.h"

#include "Platform/OpenGL/Buffers/VBO/OpenGLVertexObjectBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    uint32_t VBOElement::CalculateElementSize() const
    {
        switch (type)
        {
        case Float: return 4;
        case FloatVec2: return 8;
        case FloatVec3: return 12;
        case FloatVec4: return 16;
        }
        return 0;
    }

    uint32_t VBOElement::CalculateElementCount() const
    {
        switch (type)
        {
        case Float: return 1;
        case FloatVec2: return 2;
        case FloatVec3: return 3;
        case FloatVec4: return 4;
        }
        return 0;
    }

    std::string VBOElement::GetVBOElementTypeName(VBOElementType vboElementType)
    {
        switch (vboElementType)
        {
        case Float:return "Float";
        case FloatVec2: return "FloatVec2";
        case FloatVec3: return "FloatVec3";
        case FloatVec4: return "FloatVec4";
        }
        return "Unknown!";
    }

    VBOLayout::VBOLayout(std::initializer_list<VBOElement> elements)
    {
        m_VBOElements = elements;
        CalculateOffsetAndStride();
    }

    void VBOLayout::CalculateOffsetAndStride()
    {
        int offset = 0;
        m_Stride = 0;
        for (auto& vboElement : m_VBOElements) {
            vboElement.offset = offset;
            offset += vboElement.size;
            m_Stride += vboElement.size;
        }
    }

    const std::vector<VBOElement>& VBOLayout::GetVBOElements() const
    {
        return m_VBOElements;
    }

    uint32_t VBOLayout::GetStride() const
    {
        return m_Stride;
    }

    VertexObjectBuffer::~VertexObjectBuffer()
    {
    }

    Ref<VertexObjectBuffer> VertexObjectBuffer::Create(float* data, uint32_t size)
    {
        switch (Renderer::GetRenderingAPIType())
        {
        case RenderingAPIType::None:
            {
                Logger::Error("VertexObjectBuffer::Create | Unknown rendering api!.");
                return nullptr;
            }
        case RenderingAPIType::OpenGL:
            {
                return CreateRef<OpenGLVertexObjectBuffer>(data, size);
            }
        }
        return {};
    }
}
