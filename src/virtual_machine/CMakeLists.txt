set(LIBRARY_NAME cppel7_virtual_machine)

add_library(${LIBRARY_NAME}
        STATIC
            virtual_machine.h
            virtual_machine.cpp
)
add_library(Cppel7::VirtualMachine ALIAS ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
