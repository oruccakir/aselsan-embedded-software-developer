cmake_minimum_required(VERSION 3.16)
project(week1_memory CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Each exercise is a separate executable
add_executable(01_pointers 01_pointers/pointers.cpp)
add_executable(02_references 02_references/references.cpp)
add_executable(03_stack_heap 03_stack_vs_heap/stack_heap.cpp)
add_executable(04_raii 04_raii/raii.cpp)
add_executable(05_smart_pointers 05_smart_pointers/smart_pointers.cpp)
add_executable(06_move_semantics 06_move_semantics/move_semantics.cpp)
add_executable(07_const_correctness 07_const_correctness/const_correctness.cpp)
