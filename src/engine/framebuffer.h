#ifndef CPPEL7_FRAMEBUFFER_H
#define CPPEL7_FRAMEBUFFER_H

#include <span>

namespace cppel7 {

using Pixel = std::uint32_t;

class FrameBuffer
{
public:
    explicit FrameBuffer(std::size_t width, std::size_t height);

    std::size_t width() const;
    std::size_t height() const;

    const Pixel* data() const;

    std::span<Pixel> pixels();
    std::span<const Pixel> pixels() const;

private:
    const std::size_t m_width;
    const std::size_t m_height;

    std::vector<Pixel> m_pixels;
};

} // namespace cppel7

#endif // CPPEL7_FRAMEBUFFER_H
