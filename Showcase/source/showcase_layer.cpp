#include "pch.h"

#include "showcase_layer.h"

#include <execution>

#include "imgui.h"
#include "stb_image.h"
#include "core/application/retro_application.h"
#include "core/input/input_manager.h"
#include "core/scene/actor.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro
{
	struct tex_data
	{
		const unsigned char* data;
		int width, height, channels;

		tex_data(const unsigned char* data, int width, int height, int channels) : data(data), width(width), height(height), channels(channels) {}
	};
	showcase_layer::showcase_layer(const std::string& name) : layer(name)
	{
	}

	showcase_layer::~showcase_layer()
	{
	}

	void showcase_layer::on_layer_registered()
	{
		const shared<scene>& scene = scene::create("Showcase Scene");
		retro_application::get_application().get_scene_manager()->set_active_scene(scene);
		renderer::camera_specification camera_specification = { 50.0f, 0.01f, 1000.0f };
		m_camera = retro::create_shared<renderer::camera>(camera_specification);

		m_primes_shader = retro_application::get_application().get_assets_manager()->create_shader(
			{ "Assets/Shaders/Screen/Screen.vert","Assets/Shaders/Test/Prime.frag" });

		create_screen_vao();

		renderer::scene_renderer::set_scene(
			retro_application::get_application().get_scene_manager()->get_active_scene());
		renderer::scene_renderer::initialize(m_camera);

		std::vector<std::string> textures_path;
		const std::string path = "Assets/Models/NightCity/textures/";
		const std::string directory = path.substr(0, path.find_last_of('/'));
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			textures_path.emplace_back(entry.path().string());
		}

		/*
		std::vector<tex_data> textures_data;
		std::for_each(std::execution::par, std::begin(textures_path), std::end(textures_path), [&](std::string& file)
			{
				logger::info("Loading async tex: " + file);
		// Variables for stb.
		int width, height, channels;
		stbi_uc* data;

		// Load file using STB.
		stbi_set_flip_vertically_on_load(1);
		{
			data = stbi_load(file.c_str(), &width, &height, &channels, 0);
		}
		tex_data tex_data = { data, width, height, channels };
		textures_data.emplace_back(tex_data);
			});

		std::vector<shared<renderer::texture>> textures;
		textures.resize(textures_data.size());
		int i = 0;
		for (const tex_data& tex : textures_data)
		{
			auto asset_tex = renderer::texture::create(tex.width, tex.height, tex.channels, tex.data);
			textures.insert(textures.begin() + i, asset_tex);
			i++;
		}
		*/
		const std::vector<shared<renderer::texture>>& textures = retro_application::get_application().get_assets_manager()->create_textures(textures_path);
		logger::info("Textures: " + textures.size());
	}

	void showcase_layer::on_layer_unregistered()
	{
	}

	void showcase_layer::on_layer_updated()
	{
		//renderer::scene_renderer::begin_render();
		m_fbo->bind();
		renderer::renderer::set_renderer_state(renderer::renderer_state::depth_test, true);
		renderer::renderer::set_clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
		m_primes_shader->bind();
		renderer::renderer::clear_screen();

		const glm::vec2 screen_res = { retro_application::get_application().get_window()->get_window_specification().width,
			retro_application::get_application().get_window()->get_window_specification().height };
		m_primes_shader->set_vec_float2("uResolution", screen_res);
		m_primes_shader->set_float("uTime", renderer::renderer::get_time());
		const glm::vec2 mouse_pos = input::input_manager::get_mouse_location();
		m_primes_shader->set_vec_float2("uMousePos", mouse_pos);
		renderer::renderer::submit_command({ m_primes_shader, m_screen_vao, nullptr });
		m_primes_shader->un_bind();
		m_fbo->un_bind();

		//renderer::scene_renderer::end_render();
		ImGui::Begin("Viewport");
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(m_fbo->get_attachment_id(0)),
			ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });
		ImGui::End();
	}

	void showcase_layer::create_screen_vao()
	{
		float squareVertices[5 * 4] = {
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f		// top left
		};

		// Fill index buffer
		uint32_t squareIndices[6] = {
				0, 3, 1, // first triangle
				1, 3, 2, // second triangle
		};
		m_screen_vao = renderer::vertex_array_buffer::create();
		const auto vbo = renderer::vertex_object_buffer::create(
			squareVertices, sizeof(squareVertices));
		const auto ibo = renderer::index_buffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_screen_vao->bind();
		vbo->set_layout({ {renderer::FloatVec3, "aPos"},
										 {renderer::FloatVec2, "aTexCoord"} });
		m_screen_vao->add_vertex_buffer(vbo);
		m_screen_vao->set_index_buffer(ibo);
		m_screen_vao->un_bind();

		renderer::texture_specification final_tex_spec = {
			glm::uvec2(1920, 1080),
			renderer::texture_filtering::linear,
			renderer::texture_wrapping::clamp_edge,
			GL_RGBA, GL_RGB16F };
		m_fbo =
			renderer::frame_buffer::create({ 1920, 1080, {{"fbo", final_tex_spec}} });
	}
}