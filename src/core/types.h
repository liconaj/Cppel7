#ifndef CPPEL7_CELL_H
#define CPPEL7_CELL_H

#include <cstdint>

namespace cppel7 {

using Byte = std::uint8_t;

using Address = std::uint32_t;

using GlyphIndex = std::uint8_t;

using PaletteIndex = std::uint8_t;

using Size = std::uint32_t;

struct ColorAttr
{
    Byte packed;

    [[nodiscard]]
    PaletteIndex foreground() const noexcept
    {
        return static_cast<PaletteIndex>(packed & 0x0F);
    }

    [[nodiscard]]
    PaletteIndex background() const noexcept
    {
        return static_cast<PaletteIndex>((packed & 0xF0) >> 4);
    }
};

struct Cell
{
    GlyphIndex glyph;
    ColorAttr colorAttr;
};

} // namespace cppel7

#endif // CPPEL7_CELL_H
