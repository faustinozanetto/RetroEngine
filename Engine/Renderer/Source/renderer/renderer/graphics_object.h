#pragma once

namespace Retro::Renderer
{
	class graphics_object
	{
	public:
		/* Constructor & Destructor */
		virtual ~graphics_object() = default;

		/* Methods */
		virtual void bind() = 0;
		virtual void un_bind() = 0;

		/* Getter */
		unsigned int get_object_handle() const;

	protected:
		unsigned int m_object_handle = 0;
	};
}
