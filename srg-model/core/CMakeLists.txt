cmake_minimum_required(VERSION 3.5)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CPPS "src/*")
file(GLOB HS   "include/*")

include_directories("include")

add_library(core ${CPPS} ${HS} "include/RK4.h" "src/RK4.cpp")
