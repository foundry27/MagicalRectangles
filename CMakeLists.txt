cmake_minimum_required(VERSION 3.7)
project(DT1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp name_generator.cpp name_generator_ioc_container.cpp rect_generator.cpp)
add_executable(DT1 ${SOURCE_FILES})
