#include "framebuffer.h"

namespace cppel7 {

FrameBuffer::FrameBuffer(const std::size_t width, const std::size_t height)
    : m_width(width), m_height(height), m_pixels(width * height)
{
}

void FrameBuffer::setPixel(const int x, const int y, const PixelColor pixel)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        const std::size_t index = (y * m_width) + x;
        m_pixels.at(index) = pixel;
    }
}

std::size_t FrameBuffer::width() const
{
    return m_width;
}

std::size_t FrameBuffer::height() const
{
    return m_height;
}

std::span<const PixelColor> FrameBuffer::pixels() const
{
    return m_pixels;
}

std::span<PixelColor> FrameBuffer::pixels()
{
    return m_pixels;
}

const PixelColor* FrameBuffer::data() const
{
     return m_pixels.data();
}

} // namespace cppel7