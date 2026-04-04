cmake_minimum_required(VERSION 3.10)
project(PHPA_star)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_compile_options(-O3 -march=native)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_library(phpa_lib ${SOURCES})
target_include_directories(phpa_lib PUBLIC include)

include(FetchContent)

option(TRACY_ENABLE "" OFF)
option(TRACY_NO_EXIT "" ON)
option(TRACY_VERBOSE "" ON)

FetchContent_Declare(
    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG v0.13.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

target_link_libraries(phpa_lib PUBLIC TracyClient)

add_executable(PHPA_star src/main.cpp)
target_link_libraries(PHPA_star PRIVATE phpa_lib)

enable_testing()
file(GLOB TEST_FILES "tests/*.cpp")
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE phpa_lib)
    target_include_directories(${TEST_NAME} PRIVATE include)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()