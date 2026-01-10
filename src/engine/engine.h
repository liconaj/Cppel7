#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "core/config.h"
#include "framebuffer.h"

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

    bool m_quitRequested {false};
};

} // namespace cppel7

#endif // ENGINE_H
