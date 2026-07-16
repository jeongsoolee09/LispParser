find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(LittleLispParserTest test_main.cpp lex/test_lexer.cpp)

target_link_libraries(LittleLispParserTest
  PRIVATE
  GTest::gtest_main
  GTest::gmock
  LittleLispParserLib
)

target_compile_features(LittleLispParserTest PRIVATE cxx_std_20)

gtest_discover_tests(LittleLispParserTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
