cmake_minimum_required(VERSION 3.17)
project(ProgrammingAssignment_2)

find_package(MPI)
include_directories(${MPI_INCLUDE_PATH})

# set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_COMPILER mpic++)

add_executable(ProgrammingAssignment_2 main.cpp)