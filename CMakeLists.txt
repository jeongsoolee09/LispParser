cmake_minimum_required(VERSION 4.0)
project(LittleLispParser VERSION 1.0.0 LANGUAGES CXX)

add_library(LittleLispParserLib STATIC)
add_subdirectory(src)
target_compile_features(LittleLispParserLib PUBLIC cxx_std_20)
target_include_directories(LittleLispParserLib PUBLIC include)

add_executable(LittleLispParser src/main.cpp)
target_link_libraries(LittleLispParser PRIVATE LittleLispParserLib)

