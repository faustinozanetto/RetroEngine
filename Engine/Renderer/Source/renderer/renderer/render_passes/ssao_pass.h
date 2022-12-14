#pragma once

#include "render_pass.h"

namespace retro::renderer
{
	class ssao_pass : public render_pass
	{
	public:
		ssao_pass();
		~ssao_pass() override;

		void begin_pass() override;
	};
}