cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0077 NEW)  # Enables modern CMake variable behavior

if(MSVC)
    set(DEBUG_BUILD_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/Debug/logs") # Adjust if necessary
else()
    set(DEBUG_BUILD_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/Debug/logs") # Modify for your platform if needed
endif()

add_custom_target(clean_debug
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEBUG_BUILD_FOLDER}
    COMMENT "Removing Debug folder before building..."
)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OGL/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OGL/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window/*.rc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/*.rc"
)


# Fetch Assimp
set(ASSIMP_BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY "https://github.com/assimp/assimp.git"
    GIT_TAG "master"
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(assimp)

# Link Assimp



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

)

target_link_libraries(OGL PRIVATE assimp)

# Include directories
target_include_directories(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/OGL)
target_include_directories(OGL PRIVATE ${glew_SOURCE_DIR}/include)
target_include_directories(OGL PRIVATE ${glm_SOURCE_DIR})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRESOURCE_DIR="${CMAKE_SOURCE_DIR}/src/resources")

target_precompile_headers(OGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/OGL/precomp.h)


add_custom_command(TARGET OGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/Aadis GFX LAB/aditya_rtr_2024/01-RTR_OpenGL/01-Windows/02-PP/15-ModelLoading/build/_deps/assimp-build/bin/Debug/assimp-vc143-mtd.dll"
    $<TARGET_FILE_DIR:OGL>
    COMMENT "Copying Assimp DLL to output directory"
)
