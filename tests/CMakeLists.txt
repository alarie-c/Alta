add_executable(tests tests.cpp)
target_include_directories(tests
    PUBLIC ${PROJECT_SOURCE_DIR}/include  # From the compiler
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}   # For doctest.hpp
)
target_link_libraries(tests PUBLIC altac)