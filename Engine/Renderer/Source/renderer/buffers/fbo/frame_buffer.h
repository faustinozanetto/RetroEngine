#pragma once

#include <map>

#include "core/base.h"
#include "renderer/texture/texture.h"
#include "renderer/renderer/graphics_object.h"

namespace retro::renderer
{
    enum class frame_buffer_attachment_format
    {
        rgba8 = 0,
        rgba16f = 1,
        redint = 2,
        depth32f = 3
    };

    struct frame_buffer_texture_specification
    {
        std::string name;
        texture_specification specification;

        frame_buffer_texture_specification() = default;

        frame_buffer_texture_specification(std::string name,
                                           texture_specification specification) : name(std::move(name)),
            specification(std::move(specification))
        {
        }
    };

    struct frame_buffer_specification
    {
        uint32_t width;
        uint32_t height;

        std::vector<frame_buffer_texture_specification> attachments;

        frame_buffer_specification() : width(1920), height(1080), attachments({})
        {
        }

        frame_buffer_specification(uint32_t width, uint32_t height,
                                   std::initializer_list<frame_buffer_texture_specification> attachments) :
            width(width),
            height(height), attachments(attachments)
        {
        }
    };

    class frame_buffer : public graphics_object
    {
    public:
        /* Destructor */
        ~frame_buffer() override;

        /* Methods */
        void bind() override = 0;
        void un_bind() override = 0;

        virtual void add_texture_attachment(
            const frame_buffer_texture_specification& frame_buffer_texture_specification) = 0;

        virtual void resize(uint32_t newWidth, uint32_t newHeight) = 0;
        virtual uint32_t get_attachment_id(uint32_t slot = 0) = 0;
        virtual std::vector<uint32_t> get_attachments() = 0;
        virtual std::map<uint32_t, frame_buffer_texture_specification> get_attachments_specifications() = 0;
        virtual uint32_t get_depth_attachment_id() = 0;
        virtual uint32_t get_width() const = 0;
        virtual uint32_t get_height() const = 0;

        /* Instantiate */
        static shared<frame_buffer> create(const frame_buffer_specification& frame_buffer_specification);
    };
}
