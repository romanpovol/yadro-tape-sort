cmake_minimum_required(VERSION 3.22)

project(tape_sort)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
        GIT_TAG "f7320141120f720aecc4c32be25586e7da9eb978"
)
FetchContent_GetProperties(yaml-cpp)
FetchContent_Populate(yaml-cpp)
add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})

include_directories(include)

add_executable(tape_sort
        src/config.cpp
        src/tape.cpp
        src/sorter.cpp
        main.cpp
)

add_executable(tape_sort_tests
        src/config.cpp
        src/tape.cpp
        src/sorter.cpp
        tests/tests.cpp
        tests/utils.cpp
        tests/doctest_main.cpp
)

target_link_libraries(tape_sort PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(tape_sort_tests PUBLIC yaml-cpp::yaml-cpp)