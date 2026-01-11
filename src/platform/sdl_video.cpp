#include "sdl_video.h"
#include "core/constants.h"

#include <cstring>
#include <stdexcept>

namespace cppel7 {

SdlVideo::SdlVideo() = default;

SdlVideo::~SdlVideo()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
}

void SdlVideo::initialize(const Config& config)
{
    constexpr int LOGICAL_SIZE_SCALE {2};

    m_logicalWidth = config.width * CELL_SIZE * LOGICAL_SIZE_SCALE;
    m_logicalHeight = config.height * CELL_SIZE * LOGICAL_SIZE_SCALE;

    const int windowWidth {m_logicalWidth * static_cast<int>(config.scale)};
    const int windowHeight {m_logicalHeight * static_cast<int>(config.scale)};

    m_window = SDL_CreateWindow(config.title.c_str(), windowWidth, windowHeight,
                                SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetWindowMinimumSize(m_window, m_logicalWidth, m_logicalHeight);

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (m_renderer == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    // Make the renderer content scaled up by integer multiples to fit the output resolution
    SDL_SetRenderLogicalPresentation(m_renderer, m_logicalWidth, m_logicalHeight,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_SetDefaultTextureScaleMode(m_renderer, SDL_SCALEMODE_PIXELART);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_BGRX32, SDL_TEXTUREACCESS_STREAMING,
                                  m_logicalWidth / LOGICAL_SIZE_SCALE,
                                  m_logicalHeight / LOGICAL_SIZE_SCALE);
    if (m_texture == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(TPS).c_str());
}

void SdlVideo::present(const FrameBuffer& frameBuffer) const
{
    SDL_UpdateTexture(m_texture, nullptr, frameBuffer.data(),
                      frameBuffer.width() * sizeof(PixelColor));

    SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void SdlVideo::updateWindowTitle(const std::string& title) const
{
    SDL_SetWindowTitle(m_window, title.c_str());
}

} // namespace cppel7