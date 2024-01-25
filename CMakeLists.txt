# Project definition
cmake_minimum_required(VERSION 3.1)
project(a)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")
set(FREETYPE_FT2BUILD_INCLUDE_DIR "/usr/include/freetype2/")
set(FREETYPE_INCLUDE_DIR "/usr/include/freetype2/")
set(FREETYPE_LIBRARY_DBG "/usr/lib/x86_64-linux-gnu/libfreetype.so")
set(FREETYPE_LIBRARY_REL "/usr/lib/x86_64-linux-gnu/libfreetype.so")


# Executable definition and properties
add_executable(a ${SOURCES})
include_directories (/usr/include/freetype2)
link_libraries("/usr/lib/x86_64-linux-gnu/libfreetype.so")
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# glm 
set(GLM_DIR "${LIB_DIR}/glm")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")

# MAC
include(FindPkgConfig)
if (NOT APPLE)
  pkg_check_modules(GL REQUIRED gl)
  include_directories(${GL_INCLUDE_DIRS})
  target_link_libraries (${PROJECT_NAME} ${GL_LIBRARIES})
endif()

if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# GLEW
pkg_check_modules(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GLEW_LIBRARIES})