cmake_minimum_required(VERSION 3.16)
project(HW1 VERSION 1.0.0 LANGUAGES C CXX)
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Define source files
file(GLOB SRC_FILES
    src/*.cpp
    src/*.c
)
# Define header files
file(GLOB HEADER_FILES
    include/*.h
)
# Choose GLFW library based on compiler
if(MSVC)
    # Find MSVC version
    if(MSVC_VERSION GREATER_EQUAL 1930)  # VS 2022
        set(GLFW_LIB_DIR lib-vc2022)
    elseif(MSVC_VERSION GREATER_EQUAL 1920)  # VS 2019
        set(GLFW_LIB_DIR lib-vc2019)
    elseif(MSVC_VERSION GREATER_EQUAL 1910)  # VS 2017
        set(GLFW_LIB_DIR lib-vc2017)
    elseif(MSVC_VERSION GREATER_EQUAL 1900)  # VS 2015
        set(GLFW_LIB_DIR lib-vc2015)
    else()  # VS 2013 or older
        set(GLFW_LIB_DIR lib-vc2013)
    endif()
    # Add GLFW import library
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/${GLFW_LIB_DIR}/glfw3.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include
    )
    # For DLL version
    set(GLFW_DLL ${CMAKE_SOURCE_DIR}/${GLFW_LIB_DIR}/glfw3.dll)
elseif(MINGW)
    set(GLFW_LIB_DIR lib-mingw-w64)
    # Add GLFW import library
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/${GLFW_LIB_DIR}/libglfw3.a
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include
    )
    # For DLL version
    set(GLFW_DLL ${CMAKE_SOURCE_DIR}/${GLFW_LIB_DIR}/glfw3.dll)
endif()
# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/include
)
# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
)
# Platform-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        gdi32
    )
    # Copy GLFW DLL to output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_DLL}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    # Copy assets to output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    )
    # Copy shaders to output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    )
endif()
# For Linux/macOS (if you plan to support them)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL
        X11
        pthread
        dl
    )
endif()
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
# Install targets (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
# Copy assets during installation
install(DIRECTORY assets/ DESTINATION assets)
install(DIRECTORY shaders/ DESTINATION shaders)
