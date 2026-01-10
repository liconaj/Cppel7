set(CPPEL7_EXECUTABLE_NAME cppel7)

add_executable(${CPPEL7_EXECUTABLE_NAME}
        cppel7.cpp
)

add_subdirectory(core)
add_subdirectory(virtual_machine)
add_subdirectory(engine)
add_subdirectory(platform)

target_link_libraries(${CPPEL7_EXECUTABLE_NAME}
        PRIVATE
            Cppel7::Platform
            SDL3::SDL3-shared
)

# Copy SDL
if (SDL_VENDORED)
    add_custom_command(
            TARGET ${CPPEL7_EXECUTABLE_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${CPPEL7_EXECUTABLE_NAME}>
            VERBATIM
    )
    # Configure rpath in Linux/macOS
    if (APPLE)
        set_target_properties(${CPPEL7_EXECUTABLE_NAME} PROPERTIES
                INSTALL_RPATH "@executable_path"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
    elseif (UNIX)
        set_target_properties(${CPPEL7_EXECUTABLE_NAME} PROPERTIES
                INSTALL_RPATH "$ORIGIN"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif ()
endif ()