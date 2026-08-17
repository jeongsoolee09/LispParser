#include "AST.hpp"
#include "token.hpp"
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>

Atom parse_atom(std::vector<LispToken>::const_iterator &it,
                std::vector<LispToken>::const_iterator end) {
  if (const auto &symbol = std::get_if<Symbol>(&*it)) {
    return SymbolNode{ symbol };
  } else if (const auto &literal = std::get_if<Literal>(&*it)) {
    return LiteralNode{ literal };
  } else {
    throw std::runtime_error("Unreachable");
  }
}

/**
 * If LParen, call (push), if RParen, return (pop).

 * a b c) (d e))
 */
List parse_list(std::vector<LispToken>::const_iterator &it,
                std::vector<LispToken>::const_iterator end) {
  std::vector<Node> result;
  bool seen_rparen = false;
  while (it != end) { // Prevent undefined behavior!
    if (const auto &delimiter = std::get_if<Delimiter>(&*it)) {
      if (delimiter->get_kind() == TokenKind::LPAREN) {
        List inner_list_parsed = parse_list(++it, end);
        result.push_back(inner_list_parsed);
      } else if (delimiter->get_kind() == TokenKind::Eof) {
        break; // unsuccessful break
      } else { // It's an RPAREN
        ++it;
        seen_rparen = true;
        break; // successful break
      }
    } else {
      Atom atom_parsed = parse_atom(it, end);
      result.push_back(atom_parsed);
    }
    ++it;
  }
  if (!seen_rparen) {
    if (it != end) { // unmatched parenthesis: EOF is met
      std::cerr << "Expected RParen at " << to_string(it->get_kind())
                << std::endl;
    }
    /* unmatched parenthesis: Internal error: Lexer error. Lexer never appended
     * an EOF */
    throw std::runtime_error("Unexpected end of token stream");
  }
  return result;
}

// clang-format off
/*

TODO: Write these and use them in place of low-level machinery:

1. `expect_advance`: Advance the iterator if the token is expected, signal
error otherwise.

bool expect_advance(std::vector<LispToken>::const_iterator &it,
										std::vector<LispToken>::const_iterator end,
										TokenKind expected_token_kind)


- Returns true if the token (*it) was an expected one
- Returns false if the token (*it) was an unexpected one
- Advances `it` if the token was an expected one

2. `expect`: See if the given token (commonly the next token) is of the expected kind.

bool expect(LispToken current_token, TokenKind expected_token_kind)

- Returns true if the current token was an expected one
- Returns false if the current token was an unexpected one
*/

/*
- DONE 1: More LParens than RParens
- TODO 2: More RParens than LParens
- WORKING 3: Update tests (add some tests for the parser).
- TODO 4: Grammar-guided fuzzer: type 'a list = nil | cons of 'a * list
 */

// clang-format on

Program parse(std::vector<LispToken> program) {
  std::vector<Node> result;
  for (auto it = program.cbegin(); it == program.cend(); it++) {
    // const Token& token = *it; // slicing!
    if (auto delimiter = std::get_if<Delimiter>(&*it)) {
      if (auto lparen = std::get_if<LParen>(delimiter)) {
        // 1. Collect through RParen
        // 2. call parse on each
        // 3. Skip through RParen
        List list_parsed = parse_list(++it, program.cend());
        result.push_back(list_parsed);
      }
    } else {
      // collect the tokens until the next rparen token is met
      // ^ advance the iterator through rparen
      // pass the collected sequence to parse_list.
      Atom atom_parsed = parse_atom(it, program.cend());
      result.push_back(atom_parsed);
    }
  }
  return result;
}
