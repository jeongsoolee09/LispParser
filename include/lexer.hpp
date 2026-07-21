#include <cctype>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

enum class Keyword { LPAREN, RPAREN, QUOTE, ATOM, EQ, CAR, CDR, CONS, COND };

struct Symbol {
  std::string value;
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value == rhs.value;
}

using LispToken = std::variant<Keyword, Symbol>;

// 👇 `inline` is a suggestion!
// Keyword is essentially an integer and it's not worth the
// borrow
inline std::string to_string(Keyword token) {
  switch (token) {
  case Keyword::LPAREN:
    return "LPAREN";
  case Keyword::RPAREN:
    return "RPAREN";
  case Keyword::QUOTE:
    return "QUOTE";
  case Keyword::ATOM:
    return "ATOM";
  case Keyword::EQ:
    return "EQ";
  case Keyword::CAR:
    return "CAR";
  case Keyword::CDR:
    return "CDR";
  case Keyword::CONS:
    return "CONS";
  case Keyword::COND:
    return "COND";
  }
}

inline std::string pp(Keyword token) {
  switch (token) {
  case Keyword::LPAREN:
    return "(";
  case Keyword::RPAREN:
    return ")";
  case Keyword::QUOTE:
    return "quote";
  case Keyword::ATOM:
    return "atom";
  case Keyword::EQ:
    return "eq";
  case Keyword::CAR:
    return "car";
  case Keyword::CDR:
    return "cdr";
  case Keyword::CONS:
    return "cons";
  case Keyword::COND:
    return "cond";
  }
}

/**
 * Constructs a `Keyword` out of a given string. It is the inverse of `pp`.
 */
inline std::optional<Keyword> from_string(std::string_view string) {
  /* TODO: change this to use the Times 33 hash and use that in the switch
   * statement */
  if (string == "(") {
    return Keyword::LPAREN;
  } else if (string == ")") {
    return Keyword::RPAREN;
  } else if (string == "quote") {
    return Keyword::QUOTE;
  } else if (string == "atom") {
    return Keyword::QUOTE;
  } else if (string == "eq") {
    return Keyword::EQ;
  } else if (string == "car") {
    return Keyword::CAR;
  } else if (string == "cdr") {
    return Keyword::CDR;
  } else if (string == "cons") {
    return Keyword::CONS;
  } else if (string == "cond") {
    return Keyword::COND;
  } else {
    return std::nullopt;
  }
}

inline std::string to_string(const Symbol &symbol) { return symbol.value; }

inline std::string to_string(LispToken token) {
  return std::visit(
      [](auto &actual_token) {
        return to_string(actual_token);
      }, // passing an inline lambda to a template
      token);
}

std::vector<LispToken> lex(std::string_view source);
