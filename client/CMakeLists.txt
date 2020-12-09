cmake_minimum_required(VERSION 3.17)
project(client)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS system window graphics)
find_package(Threads REQUIRED)

add_executable(client main.cpp)
target_link_libraries(client sfml-system sfml-window sfml-graphics
        sfml-network Threads::Threads)