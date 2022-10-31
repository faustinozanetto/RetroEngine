#include "pch.h"

#include "lighting_environment.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/application/retro_application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace retro::renderer
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left            
    };

    lighting_environment::lighting_environment(const lighting_environment_specification& lighting_environment_specification)
    {
        m_lighting_environment_specification = lighting_environment_specification;
        m_skybox_shader = shader::create("Assets/Shaders/Skybox/Skybox.vert",
                                         "Assets/Shaders/Skybox/Skybox.frag");
        m_equirectangular_to_cubemap_shader = shader::create("Assets/Shaders/Skybox/Equirectangular.vert",
                                                             "Assets/Shaders/Skybox/Equirectangular.frag");
        m_irradiance_convolution_shader = shader::create("Assets/Shaders/Skybox/IrradianceConvolution.vert",
                                                         "Assets/Shaders/Skybox/IrradianceConvolution.frag");
        m_prefilter_shader = shader::create("Assets/Shaders/Skybox/Prefilter.vert",
                                            "Assets/Shaders/Skybox/Prefilter.frag");
        m_brdf_lut_shader = shader::create("Assets/Shaders/Skybox/BRDFLut.vert",
                                           "Assets/Shaders/Skybox/BRDFLut.frag");

        /* Setup Skybox Cube */
        m_skybox_vao = vertex_array_buffer::create();
        const shared<vertex_object_buffer> vbo = vertex_object_buffer::create(
            vertices, sizeof(vertices));
        m_skybox_vao->bind();
        vbo->set_layout({
            {FloatVec3, "aPos"},
            {FloatVec3, "aNormal"},
            {FloatVec2, "aTexCoord"}
        });
        m_skybox_vao->add_vertex_buffer(vbo);
        m_skybox_vao->un_bind();

        /* Setup Skybox Quad */
        m_quad_vao = vertex_array_buffer::create();
        float quad[] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left 
        };
        const shared<vertex_object_buffer> quad_vbo = vertex_object_buffer::create(quad, sizeof(quad));
        quad_vbo->set_layout({
            {FloatVec3, "aPos"},
            {FloatVec2, "aTexCoord"},
        });
        m_quad_vao->add_vertex_buffer(quad_vbo);
        unsigned int quadIndices[] = {
            0, 3, 1, // first triangle
            1, 3, 2 // second triangle
        };
        shared<index_buffer> quad_ibo = index_buffer::create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
        m_quad_vao->set_index_buffer(quad_ibo);

        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        const glm::mat4 captureViews[] =
        {
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };
        

        // Setup textures and frame buffer.
        setup_capture_frame_buffer();
        convert_to_cubemap(captureProjection, captureViews);
        setup_irradiance_map(captureProjection, captureViews);
        setup_prefilter_map(captureProjection, captureViews);
        setup_brdf_lut();
    }

    lighting_environment::~lighting_environment()
    {
    }

    void lighting_environment::render_environment() const
    {
        m_skybox_shader->bind();
        glBindTextureUnit(0, envCubemap);
        m_skybox_shader->set_mat4("view", m_view);
        m_skybox_shader->set_mat4("projection", m_projection);
        render_environment_cube();
        m_skybox_shader->un_bind();
    }

    void lighting_environment::set_view_projection(const glm::mat4& view, const glm::mat4& projection)
    {
        m_view = view;
        m_projection = projection;
    }

    void lighting_environment::render_environment_cube() const
    {
        m_skybox_vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void lighting_environment::render_environment_quad() const
    {
        m_quad_vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void lighting_environment::setup_capture_frame_buffer()
    {
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_lighting_environment_specification.capture_size, m_lighting_environment_specification.capture_size);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_lighting_environment_specification.capture_size, m_lighting_environment_specification.capture_size, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void lighting_environment::convert_to_cubemap(const glm::mat4& projection, const glm::mat4 view[])
    {
        //Converting hdr sky to cube map
        m_equirectangular_to_cubemap_shader->bind();
        m_equirectangular_to_cubemap_shader->set_mat4("projection", projection);

        m_lighting_environment_specification.cubemap->bind(0);

        glViewport(0, 0, m_lighting_environment_specification.capture_size, m_lighting_environment_specification.capture_size); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_equirectangular_to_cubemap_shader->set_mat4("view", view[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap,
                                   0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render_environment_cube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_equirectangular_to_cubemap_shader->un_bind();

        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void lighting_environment::setup_irradiance_map(const glm::mat4& projection, const glm::mat4 view[])
    {
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_lighting_environment_specification.irradiance_size, m_lighting_environment_specification.irradiance_size, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_lighting_environment_specification.irradiance_size, m_lighting_environment_specification.irradiance_size);

        m_irradiance_convolution_shader->bind();
        m_irradiance_convolution_shader->set_mat4("projection", projection);
        glBindTextureUnit(0, envCubemap);

        glViewport(0, 0, m_lighting_environment_specification.irradiance_size, m_lighting_environment_specification.irradiance_size); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_irradiance_convolution_shader->set_mat4("view", view[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render_environment_cube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void lighting_environment::setup_prefilter_map(const glm::mat4& projection, const glm::mat4 view[])
    {
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_lighting_environment_specification.prefilter_size, m_lighting_environment_specification.prefilter_size, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        m_prefilter_shader->bind();
        m_prefilter_shader->set_mat4("projection", projection);
        glBindTextureUnit(0, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 6;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // resize framebuffer according to mip-level size.
            unsigned int mipWidth = m_lighting_environment_specification.prefilter_size * std::pow(0.5, mip);
            unsigned int mipHeight = m_lighting_environment_specification.prefilter_size * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            m_prefilter_shader->set_float("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                m_prefilter_shader->set_mat4("view", view[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                       prefilterMap, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                render_environment_cube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void lighting_environment::setup_brdf_lut()
    {
        glGenTextures(1, &brdfLUTTexture);

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_lighting_environment_specification.brdf_lut_size, m_lighting_environment_specification.brdf_lut_size, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_lighting_environment_specification.brdf_lut_size, m_lighting_environment_specification.brdf_lut_size);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        glViewport(0, 0, m_lighting_environment_specification.brdf_lut_size, m_lighting_environment_specification.brdf_lut_size);
        m_brdf_lut_shader->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_environment_quad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    shared<lighting_environment> lighting_environment::create(const lighting_environment_specification& lighting_environment_specification)
    {
        return create_shared<lighting_environment>(lighting_environment_specification);
    }
}
