#include "token.hpp"
#include <cctype>
#include <string>

std::string to_string(TokenKind token) {
  switch (token) {
  case TokenKind::QUOTE:
    return "QUOTE";
  case TokenKind::ATOM:
    return "ATOM";
  case TokenKind::EQ:
    return "EQ";
  case TokenKind::CAR:
    return "CAR";
  case TokenKind::CDR:
    return "CDR";
  case TokenKind::CONS:
    return "CONS";
  case TokenKind::COND:
    return "COND";
  case TokenKind::UserSymbol:
    return "UserSymbol";
  case TokenKind::IntLiteral:
    return "IntLiteral";
  case TokenKind::TLiteral:
    return "TLiteral";
  case TokenKind::NilLiteral:
    return "NilLiteral";
  case TokenKind::StringLiteral:
    return "StringLiteral";
  case TokenKind::LPAREN:
    return "LPAREN";
  case TokenKind::RPAREN:
    return "RPAREN";
  case TokenKind::WHITESPACE:
    return "WHITESPACE";
  }
}

/**
 * Constructs a `KeywordSymbol` out of a given string. It is the inverse of
 * `pp`.
 */
std::optional<TokenKind> from_string(std::string_view keyword_name) {
  /* TODO: change this to use the Times 33 hash and use that in the switch
   * statement */
  if (keyword_name == "quote") {
    return TokenKind::QUOTE;
  } else if (keyword_name == "atom") {
    return TokenKind::ATOM;
  } else if (keyword_name == "eq") {
    return TokenKind::EQ;
  } else if (keyword_name == "car") {
    return TokenKind::CAR;
  } else if (keyword_name == "cdr") {
    return TokenKind::CDR;
  } else if (keyword_name == "cons") {
    return TokenKind::CONS;
  } else if (keyword_name == "cond") {
    return TokenKind::COND;
  } else {
    return std::nullopt;
  }
}
