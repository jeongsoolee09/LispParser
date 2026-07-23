#include <cctype>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

/* ==================== 1. Symbols ==================== */
/** A special type of symbol that are treated specially. */
enum class KeywordSymbol { QUOTE, ATOM, EQ, CAR, CDR, CONS, COND };

inline std::string to_string(KeywordSymbol token) {
  switch (token) {
  case KeywordSymbol::QUOTE:
    return "QUOTE";
  case KeywordSymbol::ATOM:
    return "ATOM";
  case KeywordSymbol::EQ:
    return "EQ";
  case KeywordSymbol::CAR:
    return "CAR";
  case KeywordSymbol::CDR:
    return "CDR";
  case KeywordSymbol::CONS:
    return "CONS";
  case KeywordSymbol::COND:
    return "COND";
  }
}

inline std::string pp(KeywordSymbol token) {
  switch (token) {
  case KeywordSymbol::QUOTE:
    return "quote";
  case KeywordSymbol::ATOM:
    return "atom";
  case KeywordSymbol::EQ:
    return "eq";
  case KeywordSymbol::CAR:
    return "car";
  case KeywordSymbol::CDR:
    return "cdr";
  case KeywordSymbol::CONS:
    return "cons";
  case KeywordSymbol::COND:
    return "cond";
  }
}
/** A non-keyword symbol. */
struct UserSymbol {
  std::string value;
};

inline std::string to_string(const UserSymbol &symbol) {
  return "UserSymbol(" + symbol.value + ")";
}

inline std::string pp(const UserSymbol &symbol) { return symbol.value; }

inline bool operator==(const UserSymbol &lhs, const UserSymbol &rhs) {
  return lhs.value == rhs.value;
}

using Symbol = std::variant<KeywordSymbol, UserSymbol>;
/* ==================================================== */

/* =================== 2. Literals ==================== */
/** An integer literal. */
struct IntLiteral {
  int value; // TODO: Consider floats and even non-decimal literals
  bool operator==(const IntLiteral &) const =
      default; // TODO: Uhh... will std::visit hate it?
};

inline std::string to_string(const IntLiteral &literal) {
  return std::format("IntLiteral({})", literal.value);
}

inline std::string pp(const IntLiteral &symbol) {
  return std::to_string(symbol.value);
}

/** A `t` literal. */
struct TLiteral {
  bool operator==(const TLiteral &) const =
      default; // TODO: Will this return true for all TLiteral objects?
};

inline std::string to_string(const TLiteral &literal) { return "TLiteral"; }

inline std::string pp(const TLiteral &symbol) { return "t"; }

/** A `nil` literal. */
struct NilLiteral {
  bool operator==(const NilLiteral &) const =
      default; // TODO: Will this return true for all NilLiteral objects?
};

inline std::string to_string(const NilLiteral &literal) { return "NilLiteral"; }

inline std::string pp(const NilLiteral &symbol) { return "nil"; }

/** A string literal. */
struct StringLiteral {
  std::string value;
  bool operator==(const StringLiteral &) const = default;
};

inline std::string to_string(const StringLiteral &literal) {
  return std::format("StringLiteral({})", literal.value);
}

inline std::string pp(const StringLiteral &symbol) {
  return std::format("{}", symbol.value);
}

using Literal = std::variant<IntLiteral, TLiteral, NilLiteral, StringLiteral>;
/* ==================================================== */

/* ================= 3. Delimiters ==================== */
/** A delimiter token that's purely needed for lexing. */
enum class Delimiter { LPAREN, RPAREN, WHITESPACE };

inline std::string to_string(Delimiter delimiter) {
  switch (delimiter) {
  case Delimiter::LPAREN:
    return "LPAREN";
  case Delimiter::RPAREN:
    return "RPAREN";
  case Delimiter::WHITESPACE:
    return "WHITESPACE";
  }
}

inline std::string pp(Delimiter delimiter) {
  switch (delimiter) {
  case Delimiter::LPAREN:
    return "(";
  case Delimiter::RPAREN:
    return ")";
  case Delimiter::WHITESPACE:
    return " ";
  }
}

/* ==================================================== */

using LispToken = std::variant<Symbol, Literal, Delimiter>;

/**
 * Constructs a `KeywordSymbol` out of a given string. It is the inverse of
 * `pp`.
 */
inline std::optional<KeywordSymbol> from_string(std::string_view keyword_name) {
  /* TODO: change this to use the Times 33 hash and use that in the switch
   * statement */
  if (keyword_name == "quote") {
    return KeywordSymbol::QUOTE;
  } else if (keyword_name == "atom") {
    return KeywordSymbol::ATOM;
  } else if (keyword_name == "eq") {
    return KeywordSymbol::EQ;
  } else if (keyword_name == "car") {
    return KeywordSymbol::CAR;
  } else if (keyword_name == "cdr") {
    return KeywordSymbol::CDR;
  } else if (keyword_name == "cons") {
    return KeywordSymbol::CONS;
  } else if (keyword_name == "cond") {
    return KeywordSymbol::COND;
  } else {
    return std::nullopt;
  }
}

inline std::string to_string(LispToken token) {
  return std::visit(
      [](auto &actual_token) {
        return to_string(actual_token);
      }, // passing an inline lambda to a template
      token);
}

std::vector<LispToken> lex(std::string_view source);
