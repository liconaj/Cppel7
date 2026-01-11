#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "core/config.h"
#include "video/framebuffer.h"
#include "video/screen.h"
#include "video/screen_renderer.h"
#include "virtual_machine/virtual_machine.h"

namespace cppel7 {

class Engine
{
public:
    Engine();

    const Config& config();

    void requestQuit();

    void step() const;

    void render() const;

    [[nodiscard]]
    bool quitRequested() const;

    FrameBuffer& frameBuffer();

    [[nodiscard]]
    const FrameBuffer& frameBuffer() const;

private:
    Config m_config {};
    std::unique_ptr<VirtualMachine> m_virtualMachine;
    std::unique_ptr<VideoMemoryView> m_videoMemoryView;
    std::unique_ptr<FrameBuffer> m_frameBuffer;
    std::unique_ptr<ScreenRenderer> m_screenRenderer;

    std::unique_ptr<Screen> m_screen;

    bool m_quitRequested {false};

    void uploadDefaultFontAtlas() const;
    void uploadDefaultPalette() const;
};

} // namespace cppel7

#endif // ENGINE_H
