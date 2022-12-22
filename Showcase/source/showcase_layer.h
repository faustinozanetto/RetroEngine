#pragma once

#include "core/base.h"
#include "core/layers/layer.h"
#include "core/scene/scene.h"
#include "renderer/buffers/fbo/frame_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/camera/camera.h"
#include "renderer/shader/shader.h"

namespace retro
{
	class showcase_layer final : public layer
	{
	public:
		explicit showcase_layer(const std::string& layer_name);
		~showcase_layer() override;

		void on_layer_registered() override;
		void on_layer_unregistered() override;
		void on_layer_updated() override;

		void create_screen_vao();
		void create_showcase_model();

	private:
		shared<renderer::camera> m_camera;
		shared<renderer::shader> m_shader;
		shared<renderer::vertex_array_buffer> m_screen_vao;
		bool m_is_mouse_move = false;
		glm::vec2 m_mouse_pressed_position;
	};
}
