#include "pch.h"

#include "renderer/renderer/renderer.h"
#include "core/base.h"
#include "glad/glad.h"

namespace Retro::renderer
{
	unique<renderer_api> renderer::s_renderer_api = nullptr;
	unique<rendererContext> renderer::s_context = nullptr;
	std::queue<render_command> renderer::s_command_queue = {};

	bool renderer::initialize(renderer_api_type renderer_api_type, const Window &window)
	{
		Logger::Info("renderer::Initialize | Initializing renderer.");

		// Instantiate renderer Context.
		s_context = rendererContext::create(window.GetNativeWindow());
		RETRO_CORE_ASSERT(s_context, "renderer::initialize | Unable to create renderer context");
		s_context->initialize();

		// Instantiate rendering api.
		s_renderer_api = renderer_api::create(renderer_api_type);
		RETRO_CORE_ASSERT(s_renderer_api, "renderer::initialize | Unable to create rendering api.");
		s_renderer_api->initialize();
		return true;
	}

	void renderer::set_clear_color(glm::vec4 color)
	{
		s_renderer_api->SetClearColor(color);
	}

	void renderer::clear_screen()
	{
		s_renderer_api->clear_screen();
	}

	void renderer::swap_buffers()
	{
		s_context->swap_buffers();
	}

	void renderer::poll_input()
	{
		s_context->poll_input();
	}

	void renderer::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_context->set_viewport(x, y, width, height);
	}

	void renderer::set_renderer_mode(renderer_mode renderer_mode)
	{
		s_renderer_api->set_renderer_mode(renderer_mode);
	}

	void renderer::set_renderer_state(renderer_state renderer_state, bool enabled)
	{
		s_renderer_api->set_renderer_state(renderer_state, enabled);
	}

	void renderer::bind_texture(uint32_t texture_handle, uint32_t texture_slot)
	{
		s_renderer_api->bind_texture(texture_handle, texture_slot);
	}

	bool renderer::should_close()
	{
		return s_context->should_close();
	}

	void renderer::begin()
	{
		/*SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
		ClearScreen();*/
	}

	void renderer::end()
	{
		/*
		while (!s_CommandQueue.empty())
		{
				const RenderCommand command = s_CommandQueue.front();
				ProcessRenderCommand(command);
				// Remove command from the queue.
				s_CommandQueue.pop();
		}
		*/

		swap_buffers();
		poll_input();
	}

	void renderer::submit_command(const render_command &command)
	{
		// s_CommandQueue.push(command);
		process_render_command(command);
	}

	renderer_api_type renderer::get_renderer_api_type()
	{
		return s_renderer_api->get_renderer_api_type();
	}

	double renderer::get_time()
	{
		return s_renderer_api->get_time();
	}

	void renderer::process_render_command(const render_command &command)
	{
		if (command.material)
			command.material->Bind();
		command.vao->Bind();
		// Pass the data to the rendering api and perform the actual rendering.
		uint32_t size = 0;
		if (command.vao->GetIndexBuffer())
			size = command.vao->GetIndexBuffer()->GetSize();
		s_rendering_api->ProcessRendereable(size);
		// Unbind after usage.
		command.vao->UnBind();
		if (command.material)
			command.material->UnBind();
	}
}
