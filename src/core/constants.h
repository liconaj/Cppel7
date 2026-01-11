#ifndef CPPEL7_CPPEL7_H
#define CPPEL7_CPPEL7_H

namespace cppel7 {

inline constexpr int FPS {30};
inline constexpr int CELL_SIZE {7};

inline constexpr int PALETTE_COLOR_COUNT {16};
inline constexpr int PALETTE_BYTES_PER_COLOR {4};

inline constexpr int FONT_ATLAS_ASCII_OFFSET {32};
inline constexpr int FONT_ATLAS_GLYPH_COUNT {96};
inline constexpr int FONT_GLYPH_WIDTH {CELL_SIZE};
inline constexpr int FONT_GLYPH_HEIGHT {CELL_SIZE};
inline constexpr int FONT_ATLAS_BYTES_PER_GLYPH {FONT_GLYPH_WIDTH * FONT_GLYPH_HEIGHT};

inline constexpr int SCREEN_BUFFER_BYTES_PER_CELL {2};

inline constexpr int ADDR_UNUSED_START {0x0000};
inline constexpr int ADDR_PALETTE_BASE {0x4000};
inline constexpr int ADDR_FONT_ATLAS_BASE {0x4040};
inline constexpr int ADDR_SCREEN_BUFFER_BASE {0x52A0};
inline constexpr int ADDR_FIXED_MEMORY_END {0x529F};

} // namespace cppel7

#endif // CPPEL7_CPPEL7_H
