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

// v inline is a suggestion
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

inline std::string to_string(const Symbol &symbol) { return symbol.value; }

inline std::string to_string(LispToken token) {
  return std::visit(
      [](auto &actual_token) {
        return to_string(actual_token);
      }, // passing an inline lambda to a template
      token);
}

std::vector<LispToken> lex(std::string_view source);
