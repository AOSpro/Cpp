## Project bootstrap and high-level notes
# Minimum CMake version required for project scripting/features below.
cmake_minimum_required(VERSION 3.15)

# Project name and languages used.
project(HW1 LANGUAGES C CXX)

# Require modern C++.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files from src/ (simple glob; fine for small projects).
file(GLOB SRC_FILES
    src/*.cpp
    src/*.c
)

# Main executable target.
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directory for project headers (local include/ folder).
target_include_directories(${PROJECT_NAME} PRIVATE include)


# ---------------------------------------------------------------------------
# Platform/compiler-specific GLFW selection
#
# The project ships prebuilt GLFW libraries for several toolchains (MinGW and
# multiple MSVC versions). We select the correct subdirectory at configure
# time based on the detected compiler and MSVC version. This keeps linking
# deterministic on Windows where multiple compiler ABIs exist.
# ---------------------------------------------------------------------------
if (MINGW)
    # Use the MinGW-specific prebuilt libs
    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-mingw-w64")
elseif (MSVC)
    # Pick the vcXXX folder matching the detected MSVC toolset.
    if (MSVC_VERSION GREATER_EQUAL 1930) # VS 2022
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-vc2022")
    elseif (MSVC_VERSION GREATER_EQUAL 1920) # VS 2019
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-vc2019")
    elseif (MSVC_VERSION GREATER_EQUAL 1910) # VS 2017
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-vc2017")
    elseif (MSVC_VERSION GREATER_EQUAL 1900) # VS 2015
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-vc2015")
    elseif (MSVC_VERSION GREATER_EQUAL 1800) # VS 2013
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib-vc2013")
    else()
        message(FATAL_ERROR "Unsupported MSVC version")
    endif()
else()
    # If we reach here, the configured compiler is not supported by this
    # handwritten build setup. The project expects either MinGW or MSVC on
    # Windows; other platforms would typically use system packages instead.
    message(FATAL_ERROR "Unsupported compiler: only MSVC or MinGW are configured")
endif()

# Add this directory to the linker search paths so target_link_libraries
# can find GLFW (when using the simple link form). For MinGW we prefer to
# link by full path below to avoid toolchain search path mismatches.
link_directories(${GLFW_LIB_DIR})


# ---------------------------------------------------------------------------
# Link libraries (Windows-focused)
# - On Windows we link against system OpenGL (opengl32) and the packaged
#   GLFW libraries. For MinGW we link the static archive by full path to
#   avoid differences in how the linker searches for libraries.
# - On non-Windows platforms this falls back to finding OpenGL via CMake
#   and linking 'glfw' (assumes the system has GLFW installed). This code
#   path is included primarily for portability but may not be used here.
# ---------------------------------------------------------------------------
if (WIN32)
    if (MINGW)
        # When using MinGW, link the glfw static library by full path to avoid
        # issues with differing toolchain search paths. Keep opengl32 for
        # the system GL implementation.
        target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 "${GLFW_LIB_DIR}/libglfw3.a")
    else()
        # MSVC builds can use the simple name if the library layout matches.
        target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glfw3)
    endif()
else()
    # Unix-like platforms: prefer CMake's OpenGL::GL and a system-installed
    # glfw target (this branch may never run in the packaged Windows setup).
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)
endif()


# ---------------------------------------------------------------------------
# Post-build: copy runtime DLLs next to the executable so the binary can be
# launched directly from the build directory on Windows. This copies the
# packaged glfw3.dll from the selected prebuilt folder.
# ---------------------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_LIB_DIR}/glfw3.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


# ---------------------------------------------------------------------------
# Convenience targets: create platform-specific build directories
#
# These targets are lightweight helpers that only create directory trees
# you might want to use for out-of-source builds for different toolchains.
# They do NOT attempt to configure or compile the project — they only
# create the directories so you can run cmake manually in them (or point
# an IDE at them). Example usage from the top-level build folder:
#
#   cmake --build . --target create-builds
#
# This will create one of: build-mingw, build-msvc, build-unix depending
# on the detected host/compiler. You can also explicitly create all three
# by calling the individual targets below.
# ---------------------------------------------------------------------------

add_custom_target(create-build-mingw
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/build-mingw"
    COMMENT "Create out-of-source build directory: build-mingw"
)

add_custom_target(create-build-msvc
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/build-msvc"
    COMMENT "Create out-of-source build directory: build-msvc"
)

add_custom_target(create-build-unix
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/build-unix"
    COMMENT "Create out-of-source build directory: build-unix"
)

# Top-level helper: depending on the configured compiler, make the
# platform-appropriate directory the default create-builds dependency.
if (MINGW)
    add_custom_target(create-builds DEPENDS create-build-mingw)
elseif (MSVC)
    add_custom_target(create-builds DEPENDS create-build-msvc)
else()
    add_custom_target(create-builds DEPENDS create-build-unix)
endif()

