#include "sdl_input.h"

namespace cppel7 {

SdlInput::SdlInput(Engine& engine)
    : m_engine(engine)
{
}

void SdlInput::handleEvent(const SDL_Event& event) const
{
    if (event.type == SDL_EVENT_QUIT) {
        m_engine.requestQuit();
    }
}

} // namespace cppel7