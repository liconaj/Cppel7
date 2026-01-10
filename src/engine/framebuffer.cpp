#include "framebuffer.h"

namespace cppel7 {

FrameBuffer::FrameBuffer(const std::size_t width, const std::size_t height)
    : m_width(width), m_height(height), m_pixels(width * height)
{
}

std::size_t FrameBuffer::width() const
{
    return m_width;
}

std::size_t FrameBuffer::height() const
{
    return m_height;
}

std::span<const Pixel> FrameBuffer::pixels() const
{
    return m_pixels;
}

std::span<Pixel> FrameBuffer::pixels()
{
    return m_pixels;
}

const Pixel* FrameBuffer::data() const
{
     return m_pixels.data();
}

} // namespace cppel7