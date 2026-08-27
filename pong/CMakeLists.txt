cmake_minimum_required(VERSION 3.20)
project(SDL3_Breakout VERSION 1.0 LANGUAGES CXX)

# Use C++23 as requested
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Dependencies
# CMake will look for the .pc files provided by pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)

# 2. Define Source Files
set(SOURCES
    main.cpp
    game.cpp
    entity.cpp
)

# 3. Create the Executable
add_executable(main.bin ${SOURCES})

# 4. Link Libraries
# This handles both include directories and the actual library linking
target_link_libraries(main.bin
    PRIVATE
    PkgConfig::SDL3
    PkgConfig::SDL3_IMAGE
)

# 5. Optional: Copy Assets to Build Directory
# This ensures your "assets/" folder is available where the binary runs
add_custom_command(TARGET main.bin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)