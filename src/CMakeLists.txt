set(CPPEL7_EXECUTABLE_NAME cppel7)

add_executable(${CPPEL7_EXECUTABLE_NAME}
        main.cpp
)

target_link_libraries(${CPPEL7_EXECUTABLE_NAME}
        PRIVATE SDL3::SDL3-shared
)

# Generation
include(GenerateFromTemplate)
set(GENERATE_OUTPUT_FOLDER "${CMAKE_SOURCE_DIR}/src/generated")

# Generate font atlas
generate_from_template(
        TARGET_NAME generate_font
        GENERATOR_PATH "${CMAKE_SOURCE_DIR}/tools/gen_font.py"
        INPUT_DATA_PATH "${CMAKE_SOURCE_DIR}/data/font_atlas.txt"
        TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/src/font_atlas.h.in"
        OUTPUT_PATH "${GENERATE_OUTPUT_FOLDER}/font_atlas.h"
        DEPENDENT_TARGET ${CPPEL7_EXECUTABLE_NAME}
)

# Generate color palette
generate_from_template(
        TARGET_NAME generate_palette
        GENERATOR_PATH "${CMAKE_SOURCE_DIR}/tools/gen_palette.py"
        INPUT_DATA_PATH "${CMAKE_SOURCE_DIR}/data/color_palette.txt"
        TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/src/color_palette.h.in"
        OUTPUT_PATH "${GENERATE_OUTPUT_FOLDER}/color_palette.h"
        DEPENDENT_TARGET ${CPPEL7_EXECUTABLE_NAME}
)

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