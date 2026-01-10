set(LIBRARY_NAME cppel7_platform)

add_library(${LIBRARY_NAME}
        STATIC
            sdl_video.h
            sdl_video.cpp
            sdl_input.h
            sdl_input.cpp
            app_state.h
            app_state.cpp
)
add_library(Cppel7::Platform ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
        PRIVATE
            Cppel7::Core
            Cppel7::VirtualMachine
            Cppel7::Engine
            SDL3::SDL3-shared
)

target_include_directories(${LIBRARY_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
