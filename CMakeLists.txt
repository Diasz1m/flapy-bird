cmake_minimum_required(VERSION 3.16)
project(FlappyBird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib baixa e compila automaticamente na primeira build
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(flappy_bird
    src/main.cpp
    src/Bird.cpp
    src/Pipe.cpp
    src/Game.cpp
)

target_include_directories(flappy_bird PRIVATE src)
target_link_libraries(flappy_bird PRIVATE raylib)

if(MSVC)
    target_compile_options(flappy_bird PRIVATE /utf-8 /W4)
else()
    target_compile_options(flappy_bird PRIVATE -Wall -Wextra)
endif()
