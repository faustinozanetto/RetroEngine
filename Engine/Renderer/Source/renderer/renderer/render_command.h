#pragma once

#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/materials/material.h"

namespace retro::renderer
{
    struct render_command
    {
        shared<shader> c_shader;
        shared<vertex_array_buffer> vao;
        shared<material> c_material;

        render_command(const shared<shader>& shader, const shared<vertex_array_buffer>& vao) : c_shader(shader),
            vao(vao)
        {
        }

        render_command(const shared<shader>& shader, const shared<vertex_array_buffer>& vao,
                       const shared<material>& material
        ) : c_shader(shader), vao(vao), c_material(material)
        {
        }
    };
}
