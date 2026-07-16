#include "lexer.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>

// struct iterator {
// 	T* current_item;
// }

// vectors are contiguous piece of memory.
// when it grows, it may get realloced to a different location

// If the input is not null-terminated, then it would cause an
// undefined behavior if that happen to start with one of the
// characters listed in the switch case.
// e.g. "q".
std::vector<LispToken> lex(std::string_view source) {
  std::vector<LispToken> result;
  auto it = source.cbegin();
  for (; it != source.cend(); it++) {
    char character = *it;
    switch (character) {
    case '(':
      result.emplace_back(Keyword::LPAREN);
      break;
    case ')':
      result.emplace_back(Keyword::RPAREN);
      break;
    case 'q': // We might be in "quote"
      if (*(it + 1) == 'u' && *(it + 2) == 'o' && *(it + 3) == 't' &&
          *(it + 4) == 'e') {
        result.emplace_back(Keyword::QUOTE);
        it += 4;
        break;
      }
    case 'c': // We might be in "car" / "cdr" / "cons" / "cond"
      if (*(it + 1) == 'a' && *(it + 2) == 'r') {
        result.emplace_back(Keyword::CAR);
        it += 2;
        break;
      }

      if (*(it + 1) == 'd' && *(it + 2) == 'r') {
        result.emplace_back(Keyword::CDR);
        it += 2;
        break;
      }

      if (*(it + 1) == 'o' && *(it + 2) == 'n') {
        if (*(it + 3) == 's') {
          result.emplace_back(Keyword::CONS);
          it += 3;
          break;
        }
        if (*(it + 3) == 'd') {
          result.emplace_back(Keyword::COND);
          it += 3;
          break;
        }
      }
    case 'e':
      if (*(it + 1) == 'q') {
        result.emplace_back(Keyword::EQ);
        it += 1;
        break;
      }
    case ' ':
    case '\n':
    case '\t':
      continue; // ignore all whitespaces
    default:    // very likely a symbol
      auto occurrence = std::find_if(it, source.cend(), [](char character) {
        return !std::isalnum(character);
      });
      std::ptrdiff_t distance = std::distance(it, occurrence);
			distance = std::max<std::ptrdiff_t>(1, distance);
      std::string token_value(&*it, distance);
      result.emplace_back(Symbol(token_value));
      it += distance - 1;
      break;
    }
  }
  return result;
}

// No overlaps
// q -> u ->

// Overlapping chars
// c -> a -> r
//   -> d -> r
// ...
