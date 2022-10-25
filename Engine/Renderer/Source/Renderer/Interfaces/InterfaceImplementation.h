#pragma once
#include "Core/Base.h"

namespace Retro::Renderer
{
	class InterfaceImplementation
	{
	public:
		/*  Destructor */
		virtual ~InterfaceImplementation() = default;

		/* Methods */
		virtual void InitializeInterface() = 0;
		virtual void ShutdownInterface() = 0;
		virtual void BeginInterfaceRender() = 0;
		virtual void EndInterfaceRender() = 0;

		/* Instantiate */
		static Scope<InterfaceImplementation> Create();

	private:
	};
}
