cmake_minimum_required(VERSION 3.19...4.1)
project(Cppel7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include scripts in cmake/ folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Dependencies
set(SDL_VERSION 3.4.0)
set(SDL_VERSION_SHA256 9ac2debb493e0d3e13dbd2729fb91f4bfeb00a0f4dff5e04b73cc9bac276b38d)
option(SDL_VENDORED "Use SDL vendored libraries" ON)
include(Dependencies)

add_subdirectory(src)