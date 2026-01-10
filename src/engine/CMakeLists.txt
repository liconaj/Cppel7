set(LIBRARY_NAME cppel7_engine)

add_library(${LIBRARY_NAME}
        STATIC
            framebuffer.h
            framebuffer.cpp
            engine.h
            engine.cpp
)
add_library(Cppel7::Engine ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
        PRIVATE
            Cppel7::Core
            Cppel7::VirtualMachine
)

target_include_directories(${LIBRARY_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
