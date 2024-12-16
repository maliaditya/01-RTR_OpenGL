cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0077 NEW)  # Enables modern CMake variable behavior

project(OGL)

# Find OpenGL (if applicable, can be removed for pure Win32 apps)
find_package(OpenGL REQUIRED)

include(FetchContent)


# Fetch GLEW
FetchContent_Declare(
    glew
    GIT_REPOSITORY "https://github.com/Perlmint/glew-cmake"
    GIT_TAG "master"
    GIT_SHALLOW TRUE
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glew/src  # Source files go here
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/glew/build # Build files go here
)
FetchContent_MakeAvailable(glew)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "1.0.1"
    GIT_SHALLOW TRUE
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glm/src  # Source files go here
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/glm/build # Build files go here
)
FetchContent_MakeAvailable(glm)

# Collect Source Files
file(GLOB_RECURSE OGLSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OGL/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OGL/*.h"
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
    PRIVATE libglew_static  
    PRIVATE glm::glm
)

add_custom_target(clean_debug
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/build/Debug
    COMMENT "Cleaning debug folder..."
)

# Include directories
target_include_directories(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(OGL PRIVATE ${glew_SOURCE_DIR}/include)
target_include_directories(OGL PRIVATE ${glm_SOURCE_DIR})