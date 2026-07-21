cmake_minimum_required(VERSION 4.0)
project(LittleLispParser VERSION 1.0.0 LANGUAGES CXX)
# v don't do this, remove it after you figured out a canonical way (using CMakeUserPresets.json!)
set(CMAKE_DSYMUTIL_AFTER_BUILD ON)

add_executable(LittleLispParser src/main.cpp)

add_library(LittleLispParserLib STATIC)
add_subdirectory(src)
target_compile_features(LittleLispParserLib PUBLIC cxx_std_20)
target_include_directories(LittleLispParserLib PUBLIC include)

find_package (glog REQUIRED)
target_link_libraries(LittleLispParserLib PUBLIC glog::glog)
target_compile_definitions(LittleLispParserLib PUBLIC GLOG_USE_GLOG_EXPORT)

target_link_libraries(LittleLispParser PRIVATE LittleLispParserLib)

enable_testing()
add_subdirectory(test)
