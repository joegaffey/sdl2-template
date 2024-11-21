cmake_minimum_required(VERSION 3.7)

project(SDL2Test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SourceFiles
    main.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SDL2Test ${SourceFiles})
target_link_libraries(SDL2Test ${SDL2_LIBRARIES})