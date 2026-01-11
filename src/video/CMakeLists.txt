set(LIBRARY_NAME cppel7_video)

add_library(${LIBRARY_NAME}
        STATIC
        framebuffer.h
        framebuffer.cpp
        screen.h
        screen.cpp
        video_memory_view.h
        video_memory_view.cpp
        screen_renderer.cpp
        screen_renderer.h
)
add_library(Cppel7::Video ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
        PRIVATE
        Cppel7::Core
        Cppel7::VirtualMachine
)

target_include_directories(${LIBRARY_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
