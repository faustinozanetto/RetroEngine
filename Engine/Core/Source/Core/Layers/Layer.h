#pragma once

namespace Retro
{
	class layer
	{
	public:
		/* Constructor & Destructor */
		layer(const std::string &layer_name);
		virtual ~layer() = default;

		/* Methods */
		virtual void on_layer_registered() = 0;
		virtual void on_layer_unregistered() = 0;
		virtual void on_layer_updated() = 0;
		const std::string &get_layer_name() const;

		/* Instantiate */
	private:
		std::string m_layer_name;
	};
}
