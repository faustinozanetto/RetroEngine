#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    enum VBOElementType
    {
        Float = 0,
        FloatVec2 = 1,
        FloatVec3 = 2,
        FloatVec4 = 3,
        Int = 4,
        IntVec2 = 5,
        IntVec3 = 6,
        IntVec4 = 7,
        Mat3 = 8,
        Mat4 = 9
    };

    static uint32_t CalculateElementSize(VBOElementType type) {
        switch (type)
        {
        case VBOElementType::Float:									return 4;
        case VBOElementType::FloatVec2:								return 4 * 2;
        case VBOElementType::FloatVec3:								return 4 * 3;
        case VBOElementType::FloatVec4:								return 4 * 4;
        case VBOElementType::Mat3:									return 4 * 3 * 3;
        case VBOElementType::Mat4:									return 4 * 4 * 4;
        case VBOElementType::Int:									return 4;
        case VBOElementType::IntVec2:									return 4 * 2;
        case VBOElementType::IntVec3:									return 4 * 3;
        case VBOElementType::IntVec4:									return 4 * 4;
        }
        return 0;
    }

    struct VBOElement
    {
        VBOElementType type;
        std::string name;
        uint32_t size;
        uint32_t offset;

        /* Constructors */
        VBOElement() = default;

        VBOElement(VBOElementType elementType, std::string elementName);

        uint32_t CalculateElementCount() const;
        static std::string GetVBOElementTypeName(VBOElementType vboElementType);
    };

    class VBOLayout
    {
    public:
        /* Constructors & Destructors */
        VBOLayout() = default;
        VBOLayout(std::initializer_list<VBOElement> elements);
        ~VBOLayout() = default;

        /* Methods */
        void CalculateOffsetAndStride();
        const std::vector<VBOElement>& GetVBOElements() const;
        uint32_t GetStride() const;

    private:
        std::vector<VBOElement> m_VBOElements;
        uint32_t m_Stride;
    };

    class VertexObjectBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~VertexObjectBuffer() override;

        /* Methods */
        virtual const VBOLayout& GetVBOLayout() const = 0;
        virtual void SetVBOLayout(const VBOLayout& layout) = 0;

        /* Instantiate */
        static Ref<VertexObjectBuffer> Create(float* data, uint32_t size);
    };
}
