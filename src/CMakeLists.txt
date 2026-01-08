set(CPPEL7_EXECUTABLE_NAME cppel7)

add_executable(${CPPEL7_EXECUTABLE_NAME}
        main.cpp
)

target_link_libraries(${CPPEL7_EXECUTABLE_NAME}
        PRIVATE SDL3::SDL3-shared
)

# Generate font data header
set(GENERATE_OUTPUT_FOLDER "${CMAKE_SOURCE_DIR}/src/generated")
set(FONT_DATA_GENERATOR_TOOL "${CMAKE_SOURCE_DIR}/tools/gen_font.py")
set(FONT_DATA_FILE "${CMAKE_SOURCE_DIR}/data/font_atlas.txt")
set(FONT_DATA_TEMPLATE "${CMAKE_SOURCE_DIR}/src/font_atlas.h.in")
set(FONT_DATA_HEADER "${GENERATE_OUTPUT_FOLDER}/font_atlas.h")

# Find Python Interpreter
find_package(Python3 QUIET COMPONENTS Interpreter)
if (NOT Python3_FOUND)
    if (NOT EXISTS "${FONT_DATA_HEADER}")
        message(FATAL_ERROR "Python3 is not available and '${FONT_DATA_HEADER}' does not exist.\n"
                "Install Python 3 or set Python3_EXECUTABLE.\n")
    else ()
        message(WARNING "Python3 not found. '${FONT_DATA_HEADER}' will NOT be regenerated.\n")
    endif ()
else ()
    add_custom_command(
            OUTPUT "${FONT_DATA_HEADER}"
            COMMAND ${Python_EXECUTABLE} "${FONT_DATA_GENERATOR_TOOL}" "${FONT_DATA_FILE}" "${FONT_DATA_TEMPLATE}" -o "${FONT_DATA_HEADER}"
            DEPENDS "${FONT_DATA_GENERATOR_TOOL}" "${FONT_DATA_FILE}" "${FONT_DATA_TEMPLATE}"
            COMMENT "Generating ${FONT_DATA_HEADER}..."
    )
    # Add generation of file as dependency
    add_custom_target(generate_font DEPENDS ${FONT_DATA_HEADER})
    add_dependencies(${CPPEL7_EXECUTABLE_NAME} generate_font)
endif ()

# Include generated file in target
target_include_directories(${CPPEL7_EXECUTABLE_NAME} PRIVATE "${GENERATE_OUTPUT_FOLDER}")

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