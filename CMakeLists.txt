cmake_minimum_required(VERSION 3.23)
project(pix_engine)

set(GLFW_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/")
add_subdirectory("${GLFW_SOURCES_DIR}")

set(GLAD_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_41 REPRODUCIBLE API gl:core=4.1)

add_executable(pix_engine 
    "src/main.c" 
    "src/common.h" 
    "src/file.c" 
    "src/file.h" 
    "src/gl_window.c" 
    "src/gl_window.h" 
    "src/bitmap.c" 
    "src/bitmap.h"
    "src/shader.c" 
    "src/shader.h")
target_compile_features(pix_engine PRIVATE c_std_11)

target_link_libraries(pix_engine PUBLIC glfw)
target_link_libraries(pix_engine PUBLIC glad_41)

add_custom_target(sym_data ALL
    ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/data"
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/data")
