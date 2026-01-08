include(FetchContent)

if (SDL_VENDORED)
    FetchContent_Declare(
            SDL3
            URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL3-${SDL_VERSION}.zip
            URL_HASH SHA256=${SDL_VERSION_SHA256}
    )
    set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "Disable SDL static library build" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "Disable SDL test library" FORCE)

    FetchContent_MakeAvailable(SDL3)
else ()
    find_package(SDL3 ${SDL_VERSION} REQUIRED CONFIG COMPONENTS SDL3-shared)
endif ()
