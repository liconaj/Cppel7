#ifndef CPPEL7_FRAMEBUFFER_H
#define CPPEL7_FRAMEBUFFER_H

#include <cstdint>
#include <span>
#include <vector>

namespace cppel7 {

using PixelColor = std::uint32_t;

class FrameBuffer
{
public:
    explicit FrameBuffer(std::size_t width, std::size_t height);

    void setPixel(int x, int y, PixelColor pixel);

    [[nodiscard]]
    std::size_t width() const;

    [[nodiscard]]
    std::size_t height() const;

    [[nodiscard]]
    const PixelColor* data() const;

    [[nodiscard]]
    std::span<PixelColor> pixels();

    [[nodiscard]]
    std::span<const PixelColor> pixels() const;

private:
    const std::size_t m_width;
    const std::size_t m_height;

    std::vector<PixelColor> m_pixels;
};

} // namespace cppel7

#endif // CPPEL7_FRAMEBUFFER_H
