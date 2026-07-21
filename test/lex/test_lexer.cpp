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
  auto expected_result =
      std::vector<LispToken>({Keyword::LPAREN, Keyword::RPAREN});
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, NonAlnumSingleChar) {
  std::vector<LispToken> result = lex("!");
  std::vector<LispToken> expected_result{Symbol("!")};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Quote) {
  std::vector<LispToken> result = lex("(quote car)");
  auto expected_result = std::vector<LispToken>{Keyword::LPAREN, Keyword::QUOTE,
                                                Keyword::CAR, Keyword::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cond) {
  std::vector<LispToken> result = lex("(cond a b)");
  auto expected_result =
      std::vector<LispToken>{Keyword::LPAREN, Keyword::COND, Symbol("a"),
                             Symbol("b"), Keyword::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Car) {
  std::vector<LispToken> result = lex("(car a b)");
  auto expected_result = std::vector<LispToken>{
      Keyword::LPAREN, Keyword::CAR, Symbol("a"), Symbol("b"), Keyword::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cdr) {
  std::vector<LispToken> result = lex("(cdr a b)");
  auto expected_result = std::vector<LispToken>{
      Keyword::LPAREN, Keyword::CDR, Symbol("a"), Symbol("b"), Keyword::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cons) {
  std::vector<LispToken> result = lex("(cons a (b c))");
  auto expected_result = std::vector<LispToken>{
      Keyword::LPAREN, Keyword::CONS, Symbol("a"),     Keyword::LPAREN,
      Symbol("b"),     Symbol("c"),   Keyword::RPAREN, Keyword::RPAREN};
  EXPECT_EQ(result, expected_result);
}
} // namespace
