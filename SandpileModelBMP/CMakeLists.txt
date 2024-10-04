cmake_minimum_required(VERSION 3.14)

project(
        Sandpile
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(bin)