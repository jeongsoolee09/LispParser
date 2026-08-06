#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lexer.hpp"

namespace {
class LexerTest : public ::testing::Test {};

TEST_F(LexerTest, EmptyInput) {
  std::vector<LispToken> result = lex("");
  EXPECT_TRUE(result.empty());
}

TEST_F(LexerTest, EmptyList) {
  std::vector<LispToken> result = lex("()");
  auto expected_result = std::vector<LispToken>({
      LParen{ Range{ 1, 1, 1, 2 } },
      RParen{ Range{ 1, 2, 1, 3 } },
  });
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, NonAlnumSingleChar) {
  std::vector<LispToken> result = lex("!");
  std::vector<LispToken> expected_result{ UserSymbol{ "!",
                                                      Range{ 1, 1, 1, 2 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Quote) {
  std::vector<LispToken> result = lex("(quote car)");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              QuoteKeywordSymbol{
                                  Range{ 1, 2, 1, 7 },
                              },
                              CarKeywordSymbol{ Range{ 1, 8, 1, 11 } },
                              RParen{ Range{ 1, 11, 1, 12 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cond) {
  std::vector<LispToken> result = lex("(cond a b)");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              CondKeywordSymbol{ Range{ 1, 2, 1, 6 } },
                              UserSymbol{ "a", Range{ 1, 7, 1, 8 } },
                              UserSymbol{ "b", Range{ 1, 9, 1, 10 } },
                              RParen{ Range{ 1, 10, 1, 11 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Car) {
  std::vector<LispToken> result = lex("(car a b)");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              CarKeywordSymbol{ Range{ 1, 2, 1, 5 } },
                              UserSymbol{ "a", Range{ 1, 6, 1, 7 } },
                              UserSymbol{ "b", Range{ 1, 8, 1, 9 } },
                              RParen{ Range{ 1, 9, 1, 10 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cdr) {
  std::vector<LispToken> result = lex("(cdr a b)");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              CdrKeywordSymbol{ Range{ 1, 2, 1, 5 } },
                              UserSymbol{ "a", Range{ 1, 6, 1, 7 } },
                              UserSymbol{ "b", Range{ 1, 8, 1, 9 } },
                              RParen{ Range{ 1, 9, 1, 10 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cons) {
  std::vector<LispToken> result = lex("(cons a (b c))");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              ConsKeywordSymbol{ Range{ 1, 2, 1, 6 } },
                              UserSymbol{ "a", Range{ 1, 7, 1, 8 } },
                              LParen{ Range{ 1, 9, 1, 10 } },
                              UserSymbol{ "b", Range{ 1, 10, 1, 11 } },
                              UserSymbol{ "c", Range{ 1, 12, 1, 13 } },
                              RParen{ Range{ 1, 13, 1, 14 } },
                              RParen{ Range{ 1, 14, 1, 15 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, StringLiteral) {
  std::vector<LispToken> result = lex("\"hi\"");
  auto expected_result =
      std::vector<LispToken>{ StringLiteral{ "hi", Range{ 1, 1, 1, 5 } } };
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, StringLiteralMultiple) {
  std::vector<LispToken> result = lex("(\"hi\" \"ho\")");
  auto expected_result =
      std::vector<LispToken>{ LParen{ Range{ 1, 1, 1, 2 } },
                              StringLiteral{ "hi", Range{ 1, 2, 1, 6 } },
                              StringLiteral{ "ho", Range{ 1, 7, 1, 11 } },
                              RParen{ Range{ 1, 11, 1, 12 } } };
  EXPECT_EQ(result, expected_result);
}

} // namespace
