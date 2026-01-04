include(FetchContent)

if (SDL_VENDORED)
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-${SDL_VERSION}
    )
    set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "Disable SDL static library build" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "Disable SDL test library" FORCE)

    FetchContent_MakeAvailable(SDL3)
else ()
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
endif ()
