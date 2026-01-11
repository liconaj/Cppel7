set(LIBRARY_NAME cppel7_core)

add_library(${LIBRARY_NAME} INTERFACE
        constants.h
        config.h
        types.h
)

add_library(Cppel7::Core ALIAS ${LIBRARY_NAME})

# Generation
include(GenerateFromTemplate)
set(GENERATE_OUTPUT_FOLDER "${CMAKE_SOURCE_DIR}/generated/core")

# Generate font atlas
generate_from_template(
        TARGET_NAME generate_font
        GENERATOR_PATH "${CMAKE_SOURCE_DIR}/tools/gen_font.py"
        INPUT_DATA_PATH "${CMAKE_SOURCE_DIR}/data/font_atlas.txt"
        TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/font_atlas.h.in"
        OUTPUT_PATH "${GENERATE_OUTPUT_FOLDER}/font_atlas.h"
        DEPENDENT_TARGET ${CPPEL7_EXECUTABLE_NAME}
)

# Generate color palette
generate_from_template(
        TARGET_NAME generate_palette
        GENERATOR_PATH "${CMAKE_SOURCE_DIR}/tools/gen_palette.py"
        INPUT_DATA_PATH "${CMAKE_SOURCE_DIR}/data/color_palette.txt"
        TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/color_palette.h.in"
        OUTPUT_PATH "${GENERATE_OUTPUT_FOLDER}/color_palette.h"
        DEPENDENT_TARGET ${CPPEL7_EXECUTABLE_NAME}
)

target_include_directories(${LIBRARY_NAME}
        INTERFACE
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${GENERATE_OUTPUT_FOLDER}/.."
)

target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_20)
