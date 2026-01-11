#include "video_memory_view.h"
#include "core/constants.h"

#include <cstring>

namespace cppel7 {

VideoMemoryView::VideoMemoryView(const VirtualMachine& virtualMachine, const Size screenWidth,
                                 const Size screenHeight)
    : m_virtualMachine(virtualMachine), m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
}

Size VideoMemoryView::screenWidth() const
{
    return m_screenWidth;
}

Size VideoMemoryView::screenHeight() const
{
    return m_screenHeight;
}

std::span<const Cell> VideoMemoryView::cells() const
{
    static_assert(sizeof(Cell) == 2);
    static_assert(std::is_standard_layout_v<Cell>);
    static_assert(std::is_trivially_copyable_v<Cell>);

    const Size cellCount {m_screenWidth * m_screenHeight};
    const Byte* base {m_virtualMachine.data() + ADDR_SCREEN_BUFFER_BASE};
    const auto cells {reinterpret_cast<const Cell*>(base)};

    // ReSharper disable once CppDFALocalValueEscapesFunction
    return std::span {cells, cellCount};
}

PixelColor VideoMemoryView::paletteColor(const PaletteIndex index) const
{
    const Address colorAddress {(index * PALETTE_BYTES_PER_COLOR) + ADDR_PALETTE_BASE};

    PixelColor color {};
    std::memcpy(&color, m_virtualMachine.data() + colorAddress, sizeof(color));

    return color;
}

Byte VideoMemoryView::glyphPixelValue(const GlyphIndex glyphIndex, const Size pixelIndex) const
{
    const Address glyphAddress {(glyphIndex * FONT_ATLAS_BYTES_PER_GLYPH) + ADDR_FONT_ATLAS_BASE};
    return m_virtualMachine.peek(glyphAddress + pixelIndex) & 0x01;
}

} // namespace cppel7