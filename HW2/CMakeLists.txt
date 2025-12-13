cmake_minimum_required(VERSION 3.15)
#project(HW2 C CXX)
project(HW2 LANGUAGES CXX C)
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
# Configuration options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)
# Source files
set(SOURCES
    src/main.cpp
    src/ball.cpp
    src/camera.cpp
    src/earth.cpp
    src/EBO.cpp
    src/galaxy.cpp
    src/moon.cpp
    src/orbit.cpp
    src/planet.cpp
    src/shaders.cpp
    src/stb.cpp
    src/sun.cpp
    src/textures.cpp
    src/VAO.cpp
    src/VBO.cpp
)
# Headers
set(HEADERS
    include/HW2/aosIO.h
    include/HW2/ball.h
    include/HW2/camera.h
    include/HW2/earth.h
    include/HW2/EBO.h
    include/HW2/galaxy.h
    include/HW2/main.h
    include/HW2/moon.h
    include/HW2/orbit.h
    include/HW2/planet.h
    include/HW2/shaders.h
    include/HW2/sun.h
    include/HW2/textures.h
    include/HW2/VAO.h
    include/HW2/VBO.h
)
# Shader files
set(SHADER_FILES
    shaders/default.frag
    shaders/default.vert
    shaders/galaxy.frag
    shaders/galaxy.vert
)
# Asset files
set(ASSET_FILES
    assets/textures/earth.png
    assets/textures/galaxy.png
    assets/textures/moon.png
)
# GLAD source
set(GLAD_SOURCE
    external/glad/src/glad.c
)
# Find OpenGL
find_package(OpenGL REQUIRED)
# GLFW configuration
# Option 1: Use pre-built GLFW library (adjust path based on your compiler)
if(WIN32)
    if(MSVC)
        # Find the appropriate VC version
        if(MSVC_VERSION EQUAL 1900)  # VS 2015
            set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-vc2015)
        elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)  # VS 2017
            set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-vc2017)
        elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)  # VS 2019
            set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-vc2019)
        elseif(MSVC_VERSION GREATER_EQUAL 1930)  # VS 2022
            set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-vc2022)
        else()
            set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-vc2013)
        endif()
        # Link against GLFW
        link_directories(${GLFW_LIB_DIR})
        set(GLFW_LIBRARIES glfw3.lib)
    elseif(MINGW)
        # For MinGW
        set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib-mingw-w64)
        link_directories(${GLFW_LIB_DIR})
        set(GLFW_LIBRARIES glfw3)
    endif()
    # Copy DLL to output directory
    if(EXISTS ${GLFW_LIB_DIR}/glfw3.dll)
        file(COPY ${GLFW_LIB_DIR}/glfw3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()
else()
    # For Linux/macOS, use system GLFW or build from source
    find_package(glfw3 3.3 QUIET)
    if(NOT glfw3_FOUND)
        message(STATUS "System GLFW not found, using included headers")
        # Add GLFW as a subdirectory if you have source, or just use headers
        # For now, we'll assume system installation or manual linking
        find_library(GLFW_LIBRARY NAMES glfw)
        if(GLFW_LIBRARY)
            set(GLFW_LIBRARIES ${GLFW_LIBRARY})
        else()
            message(FATAL_ERROR "GLFW not found. Please install GLFW (version 3.3 or higher)")
        endif()
    else()
        set(GLFW_LIBRARIES glfw)
    endif()
endif()
# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLAD_SOURCE} ${SHADER_FILES} ${ASSET_FILES})
# Target properties
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)
# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
)
# Platform-specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX AND NOT APPLE)
    # Linux
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        Threads::Threads
        ${X11_LIBRARIES}
        dl
    )
endif()
# Copy assets and shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)
# Copy each asset file
foreach(ASSET ${ASSET_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSET}
    )
endforeach()
# Copy each shader file
foreach(SHADER ${SHADER_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SHADER}
    )
endforeach()
# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
endif()
# Optional: Add GLM as a subdirectory if you want to build it
# add_subdirectory(external/glm)
