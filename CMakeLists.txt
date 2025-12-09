cmake_minimum_required(VERSION 3.10)

project(PHPA_star)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")  # main.cpp entfernen

add_library(phpa_lib ${SOURCES})
target_include_directories(phpa_lib PUBLIC include)


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