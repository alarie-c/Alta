cmake_minimum_required(VERSION 3.30)
project(Alta LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(altac STATIC
    src/common/span.cpp
    src/common/diagnostic.cpp
    src/common/operator.cpp
    src/lexer/token.cpp
    src/lexer/lexer.cpp
)
target_include_directories(altac PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)
add_subdirectory(src)