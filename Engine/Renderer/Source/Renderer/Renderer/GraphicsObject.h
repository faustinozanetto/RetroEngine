#pragma once

namespace Retro::Renderer
{
	class GraphicsObject
	{
	public:
		/* Constructor & Destructor */
		virtual ~GraphicsObject() = default;

		/* Methods */
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		/* Getter */
		unsigned int GetObjectHandle() const;
	protected:
		unsigned int m_ObjectHandle;
	};
}
