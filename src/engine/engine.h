#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "core/config.h"
#include "framebuffer.h"
#include "virtual_machine/virtual_machine.h"

namespace cppel7 {

class Engine
{
public:
    Engine();

    const Config& config();

    void requestQuit();
    bool quitRequested() const;

    FrameBuffer& frameBuffer();
    const FrameBuffer& frameBuffer() const;

private:
    Config m_config {};
    std::unique_ptr<FrameBuffer> m_frameBuffer;
    std::unique_ptr<VirtualMachine> m_virtualMachine;

    bool m_quitRequested {false};

    void uploadDefaultFontAtlas() const;
    void uploadDefaultPalette() const;
};

} // namespace cppel7

#endif // ENGINE_H
