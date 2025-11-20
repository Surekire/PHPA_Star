cmake_minimum_required(VERSION 3.10)

project(PHPA_star)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(PHPA_star
    src/main.cpp
)
