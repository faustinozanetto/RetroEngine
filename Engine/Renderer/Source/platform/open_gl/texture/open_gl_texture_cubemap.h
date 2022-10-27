#pragma once

#include "renderer/texture/texture_cubemap.h"
#include "core/assets/asset.h"
#include "renderer/shader/shader.h"
#include "glad/glad.h"

namespace retro::renderer
{
	class open_gl_texture_cubemap : public texture_cubemap, public asset
	{
	public:
		open_gl_texture_cubemap(const texture_specification& texture_specification);
		~open_gl_texture_cubemap() override;

		/* Methods */
		void bind() override;
		void bind(int slot) override;
		void un_bind() override;

	protected:
		texture_specification m_texture_specification;
		int m_MipMapLevels;
		int m_Channels;
		int m_Width;
		int m_Height;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}
