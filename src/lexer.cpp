// #ifdef NDEBUG
// std::cout << ...
// #endif
#include "lexer.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <unordered_map>

#include <glog/logging.h>

namespace {

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
const std::unordered_map<std::string_view, KeywordSymbol> keyword_table = {
    {"quote", KeywordSymbol::QUOTE}, {"atom", KeywordSymbol::ATOM},
    {"eq", KeywordSymbol::EQ},       {"car", KeywordSymbol::CAR},
    {"cdr", KeywordSymbol::CDR},     {"cons", KeywordSymbol::CONS},
    {"cond", KeywordSymbol::COND},
};

bool is_keyword_symbol(std::string_view symbol) {
  return keyword_table.find(symbol) != keyword_table.cend();
}
} // namespace

std::vector<LispToken> lex(std::string_view source) {
  std::vector<LispToken> result;
  auto it = source.cbegin();
  for (; it != source.cend(); it++) {
    char character = *it;
    switch (character) {
    case '(':
      result.emplace_back(Delimiter::LPAREN);
      break;
    case ')':
      result.emplace_back(Delimiter::RPAREN);
      break;
    case '"': {
      ++it; // Take off the `"` in the front
      auto occurrence = std::find_if(
          it, source.cend(), [](char character) { return character == '"'; });
      std::ptrdiff_t distance = std::distance(it, occurrence);
      std::ptrdiff_t adjusted_distance = std::max<std::ptrdiff_t>(1, distance);
      std::string token_value(&*it, adjusted_distance);
      result.emplace_back(StringLiteral(token_value));
      it += distance;
      break;
    }
    case ' ':
    case '\n':
    case '\t':
      continue; // ignore all whitespaces
    default: {  // very likely a symbol
      auto occurrence = std::find_if(it, source.cend(), [](char character) {
        return is_whitespace(character) || is_delimiter(character);
      });
      std::ptrdiff_t distance = std::distance(it, occurrence);
      DLOG(INFO) << "current_char: " << *it
                 << ", occurring char: " << *occurrence
                 << ", distance: " << distance << "\n";
      distance = std::max<std::ptrdiff_t>(1, distance);
      std::string token_value(&*it, distance);
      /* Check if it's a keyword by looking it up on the table */
      if (is_keyword_symbol(token_value)) {
        result.emplace_back(from_string(token_value).value());
      } else {
        result.emplace_back(UserSymbol(token_value));
      }
      it += distance - 1;
      break;
    }
    }
  }
  return result;
}
