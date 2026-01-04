set(CPPEL7_EXECUTABLE_NAME cppel7)

add_executable(${CPPEL7_EXECUTABLE_NAME}
        main.cpp
)

target_link_libraries(${CPPEL7_EXECUTABLE_NAME}
        PRIVATE SDL3::SDL3-shared
)

if (SDL_VENDORED)
    add_custom_command(
            TARGET ${CPPEL7_EXECUTABLE_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${CPPEL7_EXECUTABLE_NAME}>
            VERBATIM
    )
    # Configure rpath in Linux/macOS
    if(APPLE)
        set_target_properties(${CPPEL7_EXECUTABLE_NAME} PROPERTIES
                INSTALL_RPATH "@executable_path"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
    elseif(UNIX)
        set_target_properties(${CPPEL7_EXECUTABLE_NAME} PROPERTIES
                INSTALL_RPATH "$ORIGIN"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
endif()