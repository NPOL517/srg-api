cmake_minimum_required(VERSION 3.5)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CPPS "*.cpp")
file(GLOB HS   "*.h")

add_library(core ${CPPS} ${HS} core)
