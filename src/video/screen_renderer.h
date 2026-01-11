#ifndef CPPEL7_SCREEN_RENDERER_H
#define CPPEL7_SCREEN_RENDERER_H

#include "framebuffer.h"
#include "video_memory_view.h"

namespace cppel7 {

class ScreenRenderer
{
public:
    explicit ScreenRenderer(const VideoMemoryView& videoMemoryView);

    void render(FrameBuffer& frameBuffer) const;

private:
    const VideoMemoryView& m_videoMemoryView;

    void drawCell(FrameBuffer& frameBuffer, const Cell& cell, Size cellX, Size cellY) const;
};

} // namespace cppel7

#endif // CPPEL7_SCREEN_RENDERER_H
