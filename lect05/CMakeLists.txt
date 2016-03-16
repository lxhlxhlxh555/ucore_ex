cmake_minimum_required(VERSION 3.3)
project(mm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp MemBlock.h FirstFit.cpp FirstFit.h MemBlock.cpp MemoryManager.h SizeFit.h)
add_executable(mm ${SOURCE_FILES})