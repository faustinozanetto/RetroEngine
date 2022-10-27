#include "pch.h"

#include "graphics_object.h"

namespace retro::renderer
{
	uint32_t graphics_object::get_object_handle() const
	{
		return m_object_handle;
	}
}
