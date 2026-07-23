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
      std::vector<LispToken>({Delimiter::LPAREN, Delimiter::RPAREN});
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, NonAlnumSingleChar) {
  std::vector<LispToken> result = lex("!");
  std::vector<LispToken> expected_result{UserSymbol("!")};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Quote) {
  std::vector<LispToken> result = lex("(quote car)");
  auto expected_result =
      std::vector<LispToken>{Delimiter::LPAREN, KeywordSymbol::QUOTE,
                             KeywordSymbol::CAR, Delimiter::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cond) {
  std::vector<LispToken> result = lex("(cond a b)");
  auto expected_result = std::vector<LispToken>{
      Delimiter::LPAREN, KeywordSymbol::COND, UserSymbol("a"), UserSymbol("b"),
      Delimiter::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Car) {
  std::vector<LispToken> result = lex("(car a b)");
  auto expected_result = std::vector<LispToken>{
      Delimiter::LPAREN, KeywordSymbol::CAR, UserSymbol("a"), UserSymbol("b"),
      Delimiter::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cdr) {
  std::vector<LispToken> result = lex("(cdr a b)");
  auto expected_result = std::vector<LispToken>{
      Delimiter::LPAREN, KeywordSymbol::CDR, UserSymbol("a"), UserSymbol("b"),
      Delimiter::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, Cons) {
  std::vector<LispToken> result = lex("(cons a (b c))");
  auto expected_result = std::vector<LispToken>{
      Delimiter::LPAREN, KeywordSymbol::CONS, UserSymbol("a"),
      Delimiter::LPAREN, UserSymbol("b"),     UserSymbol("c"),
      Delimiter::RPAREN, Delimiter::RPAREN};
  EXPECT_EQ(result, expected_result);
}

TEST_F(LexerTest, StringLiteral) {
  std::vector<LispToken> result = lex("\"hi\"");
  auto expected_result = std::vector<LispToken>{StringLiteral("hi")};
  EXPECT_EQ(result, expected_result);
}
} // namespace
