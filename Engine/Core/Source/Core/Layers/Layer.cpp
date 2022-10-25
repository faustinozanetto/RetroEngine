#include "pch.h"
#include "Layer.h"

namespace Retro
{
	Layer::Layer(const std::string& layerName)
	{
		m_LayerName = layerName;
	}

	const std::string& Layer::GetLayerName() const
	{
		return m_LayerName;
	}
}
