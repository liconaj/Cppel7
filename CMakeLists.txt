cmake_minimum_required(VERSION 3.16...4.1)
project(Cppel7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include scripts in cmake/ folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Dependencies
set(SDL_VERSION 3.4.0)
option(SDL_VENDORED "Use SDL vendored libraries" ON)
include(Dependencies)

add_subdirectory(src)