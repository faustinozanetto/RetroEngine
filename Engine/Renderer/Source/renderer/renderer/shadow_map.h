#pragma once
#include <glad/glad.h>

namespace retro::renderer
{
    class shadow_map
    {
    public:
        shadow_map();
        ~shadow_map();

        void init(uint32_t size, uint32_t cascades);

        GLuint get_light_fbo() { return lightFBO; }
        GLuint get_light_depth_maps() { return lightDepthMaps; }

    private:
        uint32_t m_size = 0;
        GLuint lightFBO;
        GLuint lightDepthMaps;
    };
}
