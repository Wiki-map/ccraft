cmake_minimum_required(VERSION 3.10.0)

project(ccraft VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE MY_SOURCES CONFIG_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_subdirectory("${CMAKE_SOURCE_DIR}/extern/GLFW")
add_subdirectory("${CMAKE_SOURCE_DIR}/extern/GLAD")
add_subdirectory("${CMAKE_SOURCE_DIR}/extern/GLM")

add_executable(ccraft "${MY_SOURCES}")
target_include_directories(ccraft PUBLIC "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(ccraft glfw)
target_link_libraries(ccraft glad)
target_link_libraries(ccraft glm)
target_compile_definitions(ccraft PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/rez/")
