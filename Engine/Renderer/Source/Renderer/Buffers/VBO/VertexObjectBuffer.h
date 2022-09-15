﻿#pragma once

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
        IntVec2 =5,
        IntVec3 = 6,
        IntVec4 = 7,
        Mat3 = 8,
        Mat4 = 9
    };

    struct VBOElement
    {
        VBOElementType type;
        std::string name;
        uint32_t size;
        uint32_t offset;

        /* Constructors */
        VBOElement() = default;

        VBOElement(VBOElementType type, std::string name) : type(type), name(std::move(name)),
                                                            size(CalculateElementSize()),
                                                            offset(0)
        {
        }

        /* Methods */
        uint32_t CalculateElementSize() const;
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
