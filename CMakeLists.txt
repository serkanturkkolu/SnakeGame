cmake_minimum_required(VERSION 3.5)

project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SnakeGame main.cpp snake.cpp snake.h snakemap.cpp snakemap.h dot.cpp dot.h snakegame.cpp snakegame.h)

target_link_libraries( SnakeGame pthread )
