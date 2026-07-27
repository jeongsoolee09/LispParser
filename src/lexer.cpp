#include "token.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <glog/logging.h>
#include <unordered_map>

namespace {

const int INITIAL_LINE = 1;
const int INITIAL_COLUMN = 1;

bool is_whitespace(char character) {
  return character == ' ' || character == '\n' || character == '\t';
}

bool is_delimiter(char character) {
  return character == '(' || character == ')';
}

/**
 * A map from a string to a keyword. If lookup on the table fails, it means
 * the string in question is not a keyword.
 */
const std::unordered_map<std::string_view, TokenKind> keyword_table = {
    {"quote", TokenKind::QUOTE}, {"atom", TokenKind::ATOM},
    {"eq", TokenKind::EQ},       {"car", TokenKind::CAR},
    {"cdr", TokenKind::CDR},     {"cons", TokenKind::CONS},
    {"cond", TokenKind::COND},
};

bool is_keyword_symbol(std::string_view symbol) {
  return keyword_table.find(symbol) != keyword_table.cend();
}

Symbol construct_symbol(std::string_view token_value) {
	// TODO
}

} // namespace

std::vector<LispToken> lex(std::string_view source) {
  std::vector<LispToken> result;
  int current_line = 1;
  int current_column = 1;
  auto it = source.cbegin();
  for (; it != source.cend(); it++) {
    char character = *it;
    switch (character) {
    case '(':
      result.emplace_back(LParen(current_line, current_column, current_line,
                                 current_column + 1));
      current_column += 1;
      break;
    case ')':
      result.emplace_back(RParen(current_line, current_column, current_line,
                                 current_column + 1));
      current_column += 1;
      break;
    case '"': {
      ++it; // Take off the `"` in the front
      auto occurrence = std::find_if(
          it, source.cend(), [](char character) { return character == '"'; });
      std::ptrdiff_t distance = std::distance(it, occurrence);
      std::string token_value(&*it, distance);
      result.emplace_back(StringLiteral(token_value, current_line,
                                        current_column, current_line,
                                        current_column + distance + 1));
      it += distance;
      break;
    }
    case '\n':
      current_line += 1;
      current_column = 1;
      continue;
    case ' ':
    case '\t':
      continue;
    default: { // very likely a symbol
      auto occurrence = std::find_if(it, source.cend(), [](char character) {
        return is_whitespace(character) || is_delimiter(character);
      });
      std::ptrdiff_t distance = std::distance(it, occurrence);
      std::ptrdiff_t adjusted_distance = std::max<std::ptrdiff_t>(1, distance);
      std::string token_value(&*it, adjusted_distance);
      /* Check if it's a keyword by looking it up on the table */
      if (is_keyword_symbol(token_value)) {
        result.emplace_back(KeywordSymbol(
            from_string(token_value).value(), current_line, current_column,
            current_line, current_column + adjusted_distance));
      } else {
        result.emplace_back(UserSymbol(token_value, current_line,
                                       current_column, current_line,
                                       current_column + adjusted_distance));
      }
      it += distance - 1;
      break;
    }
    }
  }
  return result;
}
