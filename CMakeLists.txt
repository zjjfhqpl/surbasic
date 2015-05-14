cmake_minimum_required(VERSION 2.8)

project(surbasic C CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "debug")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CXX_FLAGS
    -fPIC
    -g
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wno-unused-parameter
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    #-Wshadow
    -Wno-shadow
    -Wwrite-strings
    -march=native
    -Wno-unused
    -std=c++0x
    -rdynamic
    )
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

add_definitions(-DMONGOOSE_NO_FILESYSTEM)
add_definitions(-DNS_ENABLE_SSL)
add_definitions(-DJSON_HAS_INT64)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(JsonCpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIRS})

include_directories(borZoi-1.0.2/src)

add_subdirectory(surbasic)
add_subdirectory(borZoi-1.0.2)
