cmake_minimum_required(VERSION 3.7)
project(DT1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp rp/vector2.hpp rp/shape.hpp rp/rectangle.hpp rp/grid.hpp rp/gridexceptions.hpp)
add_executable(DT1 ${SOURCE_FILES})