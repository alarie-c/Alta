cmake_minimum_required(VERSION 3.30)
project(Alta LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(altac STATIC
    src/common/span.cpp
)
target_include_directories(altac PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)
add_subdirectory(src)