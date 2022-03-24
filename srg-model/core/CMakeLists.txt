cmake_minimum_required(VERSION 3.5)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(core
    constants.h constants.cpp
    dephem.h dephem.cpp
    julian.h julian.cpp
    RK4.h RK4.cpp
)
