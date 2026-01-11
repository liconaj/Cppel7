#include "screen.h"
#include "core/constants.h"

// ReSharper disable CppRedundantParentheses

namespace cppel7 {

Screen::Screen(VirtualMachine& virtualMachine, const int width, const int height)
    : m_virtualMachine(virtualMachine), m_width(width), m_height(height)
{
}

void Screen::setColor(const ColorAttr color)
{
    m_currentColor = color;
}

std::optional<Cell> Screen::get(const int x, const int y) const
{
    const auto cellAddress = getCellAddress(x, y);
    if (cellAddress.has_value() == false) {
        return std::nullopt;
    }
    const GlyphIndex glyph { m_virtualMachine.peek(cellAddress.value())};
    const ColorAttr colorAttr {m_virtualMachine.peek(cellAddress.value() + 1)};

    return Cell { .glyph=glyph, .colorAttr=colorAttr };
}

void Screen::put(const int x, const int y, const GlyphIndex glyph) const
{
    const auto cellAddress = getCellAddress(x, y);
    if (cellAddress.has_value() == false) {
        return;
    }
    m_virtualMachine.poke(cellAddress.value(), glyph);
    m_virtualMachine.poke(cellAddress.value() + 1, m_currentColor.value);
}

void Screen::put(const int x, const int y, const std::span<GlyphIndex> glyphs) const
{
    for (int i {}; i < glyphs.size(); ++i) {
        put(x + i, y, glyphs[i]);
    }
}

void Screen::fill(const int x, const int y, const int w, const int h, const GlyphIndex glyph) const
{
    for (int i {x}; i < w; ++i) {
        for (int j {y}; j < h; ++j) {
            put(i, j, glyph);
        }
    }
}

std::span<const Cell> Screen::cells() const
{
    static_assert(sizeof(Cell) == 2);
    static_assert(std::is_standard_layout_v<Cell>);
    static_assert(std::is_trivially_copyable_v<Cell>);

    const std::size_t cellCount {static_cast<std::size_t>(m_width * m_height)};
    const std::byte* base {m_virtualMachine.memory().data() + ADDR_SCREEN_BUFFER_BASE};
    const Cell* cells {std::launder(reinterpret_cast<const Cell*>(base))};

    // ReSharper disable once CppDFALocalValueEscapesFunction
    return std::span{cells, cellCount};
}

bool Screen::checkCellPosition(const int x, const int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

std::optional<Address> Screen::getCellAddress(const int x, const int y) const
{
    if (checkCellPosition(x, y) == false) {
        return std::nullopt;
    }
    const int cellIndex {x + (y * m_width)};
    Address cellAddress {ADDR_SCREEN_BUFFER_BASE + (cellIndex * SCREEN_BUFFER_BYTES_PER_CELL)};
    return cellAddress;
}

const VirtualMachine& Screen::virtualMachine() const
{
    return m_virtualMachine;
}

int Screen::width() const
{
    return m_width;
}

int Screen::height() const
{
    return m_height;
}

int Screen::size() const
{
    return m_width * m_height;
}

} // namespace cppel7