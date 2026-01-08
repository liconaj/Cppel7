function(generate_from_template)
    set(options)
    set(oneValueArgs
            TARGET_NAME
            GENERATOR_PATH
            INPUT_DATA_PATH
            TEMPLATE_PATH
            OUTPUT_PATH
            DEPENDENT_TARGET
    )
    set(multiValueArgs)

    cmake_parse_arguments(GFT
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}"
            ${ARGN}
    )

    if (NOT GFT_TARGET_NAME)
        message(FATAL_ERROR "generate_from_template: TARGET_NAME is required")
    endif ()

    if (NOT GFT_GENERATOR_PATH OR NOT GFT_INPUT_DATA_PATH OR NOT GFT_TEMPLATE_PATH OR NOT GFT_OUTPUT_PATH)
        message(FATAL_ERROR "generate_from_template: GENERATOR_PATH, INPUT_DATA_PATH, TEMPLATE_PATH and OUTPUT_PATH are required")
    endif ()

    find_package(Python3 QUIET COMPONENTS Interpreter)

    if (NOT Python3_FOUND)
        if (NOT EXISTS "${GFT_OUTPUT_PATH}")
            message(FATAL_ERROR
                    "Python3 is not available and '${GFT_OUTPUT_PATH}' does not exist.\n"
                    "Install Python 3 or set Python3_EXECUTABLE.\n"
            )
        else ()
            message(WARNING
                    "Python3 not found. '${GFT_OUTPUT_PATH}' will NOT be regenerated.\n"
            )
        endif ()
    endif ()

    add_custom_command(
            OUTPUT "${GFT_OUTPUT_PATH}"
            COMMAND "${Python3_EXECUTABLE}"
                    "${GFT_GENERATOR_PATH}"
                    "${GFT_INPUT_DATA_PATH}"
                    "${GFT_TEMPLATE_PATH}"
                    -o "${GFT_OUTPUT_PATH}"
            DEPENDS "${GFT_GENERATOR_PATH}" "${GFT_INPUT_DATA_PATH}" "${GFT_TEMPLATE_PATH}"
            VERBATIM
    )

    add_custom_target(${GFT_TARGET_NAME} DEPENDS "${GFT_OUTPUT_PATH}")

    if (GFT_DEPENDENT_TARGET)
        add_dependencies(${GFT_DEPENDENT_TARGET} ${GFT_TARGET_NAME})
    endif ()

endfunction()