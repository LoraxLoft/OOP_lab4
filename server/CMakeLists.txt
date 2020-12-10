cmake_minimum_required(VERSION 3.17)
project(server)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS system window graphics)
find_package(Threads REQUIRED)

add_executable(server main.cpp Field.h Game.h Settings.h)
target_link_libraries(server sfml-system sfml-window sfml-graphics
        sfml-network Threads::Threads)