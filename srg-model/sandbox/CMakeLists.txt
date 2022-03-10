cmake_minimum_required(VERSION 3.5)

project(sandbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# core
include_directories("../core/include")

# sandbox
file(GLOB CPPS "*.cpp")

add_executable(sandbox ${CPPS})

target_link_directories(sandbox PUBLIC ${CMAKE_BINARY_DIR}/core)
target_link_libraries(sandbox core)

file(COPY ../bin/lnxp1900p2053.421 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})