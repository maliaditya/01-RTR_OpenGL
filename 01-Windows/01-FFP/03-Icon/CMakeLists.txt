cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0077 NEW)  # Enables modern CMake variable behavior

project(OGL)

# Find OpenGL (if applicable, can be removed for pure Win32 apps)
find_package(OpenGL REQUIRED)

include(FetchContent)

# Collect Source Files
file(GLOB_RECURSE OGLSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window/*.rc"
)

# Add executable
add_executable(OGL ${OGLSources})

# Specify Win32 subsystem
set_target_properties(OGL PROPERTIES
    WIN32_EXECUTABLE ON
)

# Link libraries (if using OpenGL)
target_link_libraries(OGL
    PRIVATE OpenGL::GL  # Optional for OpenGL support
)

# Include directories
target_include_directories(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
