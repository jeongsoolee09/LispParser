#include "AST.hpp"
#include "token.hpp"
#include <stdexcept>
#include <variant>
#include <vector>

Atom parse_atom(std::vector<LispToken>::const_iterator &it,
                std::vector<LispToken>::const_iterator end) {
  if (const auto &symbol = std::get_if<Symbol>(&*it)) {
    return SymbolNode{symbol};
  } else if (const auto &literal = std::get_if<Literal>(&*it)) {
    return LiteralNode{literal};
  } else {
    throw std::runtime_error("Unreachable");
  }
}

/**
 * If LParen, call (push), if RParen, return (pop).
 */
List parse_list(std::vector<LispToken>::const_iterator &it,
                std::vector<LispToken>::const_iterator end) {
  std::vector<Node> result;
  while (it != end) { // Prevent undefined behavior!
    if (const auto &delimiter = std::get_if<Delimiter>(&*it)) {
      if (delimiter->get_kind() == TokenKind::LPAREN) {
        List inner_list_parsed = parse_list(++it, end);
        result.push_back(inner_list_parsed);
      } else { // It's an RPAREN
        ++it;
        break;
      }
    } else {
      Atom atom_parsed = parse_atom(it, end);
      result.push_back(atom_parsed);
    }
    ++it;
  }
  return result;
}

/*
- TODO 1: More LParens than RParens
- TODO 2: More RParens than LParens
- TODO 3: Update tests (add some tests for the parser).


- TODO 4: Grammar-guided fuzzer: type 'a list = nil | cons of 'a * list
 */

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
