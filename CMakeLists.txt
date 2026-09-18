cmake_minimum_required(VERSION 3.14)

project(usg LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(demo examples/modules.c)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(sdl examples/sdlSimulation.c)
target_link_libraries(sdl PRIVATE ${SDL2_LIBRARIES})
