cmake_minimum_required(VERSION 3.29)
project(SchedulerSimulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(SchedulerSimulator main.cpp
        Process.cpp
        Process.h)
