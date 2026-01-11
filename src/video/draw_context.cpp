#include "draw_context.h"
#include "core/config.h"
#include "core/constants.h"

// ReSharper disable CppRedundantParentheses

namespace cppel7 {

DrawContext::DrawContext(VirtualMachine& virtualMachine, const Config& config)
    : m_virtualMachine(virtualMachine), m_width(config.width), m_height(config.height)
{
}

void DrawContext::setColor(const ColorAttr color)
{
    m_currentColor = color;
}

std::optional<Cell> DrawContext::get(const int x, const int y) const
{
    const auto cellAddress = getCellAddress(x, y);
    if (cellAddress.has_value() == false) {
        return std::nullopt;
    }
    const GlyphIndex glyph {m_virtualMachine.peek(cellAddress.value())};
    const ColorAttr colorAttr {m_virtualMachine.peek(cellAddress.value() + 1)};

    return Cell {.glyph = glyph, .colorAttr = colorAttr};
}

void DrawContext::put(const int x, const int y, const GlyphIndex glyph) const
{
    const auto cellAddress = getCellAddress(x, y);
    if (cellAddress.has_value() == false) {
        return;
    }
    m_virtualMachine.poke(cellAddress.value(), glyph);
    m_virtualMachine.poke(cellAddress.value() + 1, m_currentColor.packed);
}

void DrawContext::put(const int x, const int y, const std::span<GlyphIndex> glyphs) const
{
    for (int i {}; i < glyphs.size(); ++i) {
        put(x + i, y, glyphs[i]);
    }
}

void DrawContext::fill(const int x, const int y, const int w, const int h,
                       const GlyphIndex glyph) const
{
    for (int i {x}; i < w; ++i) {
        for (int j {y}; j < h; ++j) {
            put(i, j, glyph);
        }
    }
}

bool DrawContext::checkCellPosition(const int x, const int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

std::optional<Address> DrawContext::getCellAddress(const int x, const int y) const
{
    if (checkCellPosition(x, y) == false) {
        return std::nullopt;
    }
    const Size cellIndex {x + (y * m_width)};
    Address cellAddress {ADDR_SCREEN_BUFFER_BASE + (cellIndex * SCREEN_BUFFER_BYTES_PER_CELL)};
    return cellAddress;
}

} // namespace cppel7