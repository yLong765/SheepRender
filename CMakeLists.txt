cmake_minimum_required(VERSION 3.20)
project(SheepRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(SheepRender main.cpp sr_matrix.h sr_vector.h sr_math.h)
