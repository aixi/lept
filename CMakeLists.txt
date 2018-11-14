cmake_minimum_required(VERSION 3.0)
project(lept)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
        -g
        -Wall
        -Wextra
        -Werror
        -Wconversion
        -Wno-unused-parameter
        -Wold-style-cast
        -Wsequence-point
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wthread-safety
        -Wwrite-strings
        -march=native
        -std=c++17
        -Wno-null-dereference
        -Wno-sign-conversion
        -Wno-unused-private-field
        -fmodules
        )
if(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES
        lept/Value.h
        lept/Value.cc
        lept/Parser.h
        lept/Parser.cc
        lept/Generator.h
        lept/Generator.cc)

add_library(lept STATIC ${SOURCE_FILES})

add_executable(main main.cpp)
target_link_libraries(main lept)

add_executable(parse_unittest lept/test.cc)
target_link_libraries(parse_unittest lept boost_unit_test_framework)
add_test(NAME parse_unittest COMMAND parse_unittest)

