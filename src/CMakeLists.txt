add_executable(compiler main.cpp)
target_include_directories(compiler PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(compiler PUBLIC altac)