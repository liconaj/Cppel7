#ifndef SDL_VIDEO_H
#define SDL_VIDEO_H

#include <SDL3/SDL.h>

#include "core/config.h"
#include "engine/framebuffer.h"

namespace cppel7 {

class SdlVideo
{
public:
    explicit SdlVideo();
    ~SdlVideo();

    void initialize(const Config& config);
    void present(const FrameBuffer& frameBuffer) const;

    void updateWindowTitle(const std::string& title) const;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;

    int m_logicalWidth {0};
    int m_logicalHeight {0};
};

} // namespace cppel7

#endif // SDL_VIDEO_H
