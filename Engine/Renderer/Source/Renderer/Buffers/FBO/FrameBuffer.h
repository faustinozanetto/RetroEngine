#pragma once
#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	enum class EFrameBufferColorAttachmentFormat
	{
		RGBA8 = 0,
		RGBA16F = 1,
	};

	struct FFrameBufferColorTextureSpecification
	{
		EFrameBufferColorAttachmentFormat format;

		FFrameBufferColorTextureSpecification(EFrameBufferColorAttachmentFormat format) : format(format)
		{
		}
	};

	struct FFrameBufferSpecification
	{
		uint32_t width;
		uint32_t height;

		std::vector<FFrameBufferColorTextureSpecification> colorAttachments;

		FFrameBufferSpecification() : width(1920), height(1080), colorAttachments({})
		{
		}

		FFrameBufferSpecification(uint32_t width, uint32_t height,
		                          std::initializer_list<FFrameBufferColorTextureSpecification> colorAttachments) :
			width(width),
			height(height), colorAttachments(colorAttachments)
		{
		}
	};

	class FrameBuffer : public GraphicsObject
	{
	public:
		/* Destructor */
		~FrameBuffer() override;

		/* Methods */
		void Bind() override = 0;
		void UnBind() override = 0;

		virtual void AddColorTextureAttachment(
			const FFrameBufferColorTextureSpecification& frameBufferColorTextureAttachment) = 0;

		virtual void Resize(uint32_t newWidth, uint32_t newHeight) = 0;
		virtual uint32_t GetColorAttachmentID(uint32_t slot = 0) = 0;
		virtual uint32_t GetDepthAttachmentID() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		/* Instantiate */
		static Ref<FrameBuffer> Create(const FFrameBufferSpecification& frameBufferSpecification);
	};
}
