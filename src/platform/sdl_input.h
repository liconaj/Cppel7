#ifndef SDL_INPUT_H
#define SDL_INPUT_H
#include "SDL3/SDL_events.h"
#include "engine/engine.h"

namespace cppel7 {

class SdlInput
{
public:
    explicit SdlInput(Engine& engine);

    void handleEvent(const SDL_Event& event) const;

private:
    Engine& m_engine;
};

} // namespace cppel7

#endif // SDL_INPUT_H