#include <cctype>
#include <format>
#include <optional>
#include <string>
#include <variant>

/* =================== 0. TokenKind =================== */

enum class TokenKind {
  /* 1. Symbols */
  QUOTE,
  ATOM,
  EQ,
  CAR,
  CDR,
  CONS,
  COND,
  UserSymbol,

  /* 2. Literals */
  IntLiteral,
  TLiteral,
  NilLiteral,
  StringLiteral,

  /* 3. Delimiters */
  LPAREN,
  RPAREN,
  WHITESPACE
};

std::string to_string(TokenKind token);

/**
 * Constructs a `TokenKind` out of a given string. It is the inverse of
 * `to_string`.
 */
std::optional<TokenKind> from_string(std::string_view keyword_name);

/** Interface for a token. */
struct Token {
  Token(TokenKind token_kind, int start_line, int start_column, int end_line,
        int end_column)
      : token_kind(token_kind), start_line(start_line),
        start_column(start_column), end_line(end_line), end_column(end_column) {
  }

  const TokenKind token_kind;
  const int start_line = 0;
  const int start_column = 0;
  const int end_line = 0;
  const int end_column = 0;

  bool operator==(const Token &) const = default;
};

/* ==================== 1. Symbols ==================== */

struct QuoteKeywordSymbol : public Token {
  QuoteKeywordSymbol(int start_line, int start_column, int end_line,
                     int end_column)
      : Token(TokenKind::QUOTE, start_line, start_column, end_line,
              end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "quote"; }

  bool operator==(const QuoteKeywordSymbol &) const = default;
};

struct AtomKeywordSymbol : public Token {
  AtomKeywordSymbol(int start_line, int start_column, int end_line,
                    int end_column)
      : Token(TokenKind::ATOM, start_line, start_column, end_line, end_column) {
  }

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "atom"; }

  bool operator==(const AtomKeywordSymbol &) const = default;
};

struct EqKeywordSymbol : public Token {
  EqKeywordSymbol(int start_line, int start_column, int end_line,
                  int end_column)
      : Token(TokenKind::EQ, start_line, start_column, end_line, end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "eq"; }

  bool operator==(const EqKeywordSymbol &) const = default;
};

struct CarKeywordSymbol : public Token {
  CarKeywordSymbol(int start_line, int start_column, int end_line,
                   int end_column)
      : Token(TokenKind::CAR, start_line, start_column, end_line, end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "car"; }

  bool operator==(const CarKeywordSymbol &) const = default;
};

struct CdrKeywordSymbol : public Token {
  CdrKeywordSymbol(int start_line, int start_column, int end_line,
                   int end_column)
      : Token(TokenKind::CDR, start_line, start_column, end_line, end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "cdr"; }

  bool operator==(const CdrKeywordSymbol &) const = default;
};

struct ConsKeywordSymbol : public Token {
  ConsKeywordSymbol(int start_line, int start_column, int end_line,
                    int end_column)
      : Token(TokenKind::CONS, start_line, start_column, end_line, end_column) {
  }

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "cons"; }

  bool operator==(const ConsKeywordSymbol &) const = default;
};

struct CondKeywordSymbol : public Token {
  CondKeywordSymbol(int start_line, int start_column, int end_line,
                    int end_column)
      : Token(TokenKind::COND, start_line, start_column, end_line, end_column) {
  }

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }
  std::string pp() const { return "cond"; }

  bool operator==(const CondKeywordSymbol &) const = default;
};

struct UserSymbol : public Token {
  UserSymbol(std::string name, int start_line, int start_column, int end_line,
             int end_column)
      : Token(TokenKind::UserSymbol, start_line, start_column, end_line,
              end_column),
        name(name) {}

  std::string to_string() const {
    return std::format("{}(\"{}\"{}:{}-{}:{})", ::to_string(token_kind), name,
                       start_line, start_column, end_line, end_column);
  }
  std::string pp() const { return name; }

  bool operator==(const UserSymbol &) const = default;

  std::string name;
};

using KeywordSymbol_ =
    std::variant<QuoteKeywordSymbol, AtomKeywordSymbol, EqKeywordSymbol,
                 CarKeywordSymbol, CdrKeywordSymbol, ConsKeywordSymbol,
                 CondKeywordSymbol>;

struct KeywordSymbol : public KeywordSymbol_ {
  using KeywordSymbol_::KeywordSymbol_;

  std::string to_string() const {
    return std::visit(
        [](auto &keyword_symbol) { return keyword_symbol.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &keyword_symbol) { return keyword_symbol.pp(); },
                      *this);
  }
};

using Symbol_ = std::variant<KeywordSymbol, UserSymbol>;

struct Symbol : public Symbol_ {
  std::string to_string() const {
    return std::visit(
        [](auto &keyword_symbol) { return keyword_symbol.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &keyword_symbol) { return keyword_symbol.pp(); },
                      *this);
  }
};
/* ==================================================== */

/* =================== 2. Literals ==================== */
/** A `t` literal. */
struct TLiteral : public Token {
  TLiteral(int start_line, int start_column, int end_line, int end_column)
      : Token(TokenKind::TLiteral, start_line, start_column, end_line,
              end_column) {}

  bool operator==(const TLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }

  std::string pp() const { return "t"; }
};

/** A `nil` literal. */
struct NilLiteral : public Token {
  NilLiteral(int start_line, int start_column, int end_line, int end_column)
      : Token(TokenKind::NilLiteral, start_line, start_column, end_line,
              end_column) {}

  bool operator==(const NilLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }

  std::string pp() const { return "nil"; }
};

/** An integer literal. */
struct IntLiteral : public Token {
  IntLiteral(int value, int start_line, int start_column, int end_line,
             int end_column)
      : Token(TokenKind::IntLiteral, start_line, start_column, end_line,
              end_column),
        value(value) {}

  const int value;

  bool operator==(const IntLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({}, {}:{}-{}:{})", ::to_string(token_kind), value,
                       start_line, start_column, end_line, end_column);
  }

  std::string pp() const { return std::to_string(value); }
};

/** A string literal. */
struct StringLiteral : public Token {
  StringLiteral(std::string value, int start_line, int start_column,
                int end_line, int end_column)
      : Token(TokenKind::StringLiteral, start_line, start_column, end_line,
              end_column),
        value(value) {}

  const std::string value;

  bool operator==(const StringLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({}, {}:{}-{}:{})", ::to_string(token_kind), value,
                       start_line, start_column, end_line, end_column);
  }

  std::string pp() const { return value; }
};

using Literal_ = std::variant<TLiteral, NilLiteral, IntLiteral, StringLiteral>;

struct Literal : public Literal_ {
  using Literal_::Literal_;

  std::string to_string() const {
    return std::visit([](auto &literal) { return literal.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &literal) { return literal.pp(); }, *this);
  }
};
/* ==================================================== */

/* ================= 3. Delimiters ==================== */
/** A delimiter token that's purely needed for lexing. */
struct LParen : public Token {
  LParen(int start_line, int start_column, int end_line, int end_column)
      : Token(TokenKind::LPAREN, start_line, start_column, end_line,
              end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }

  std::string pp() const { return "("; }
};

struct RParen : public Token {
  RParen(int start_line, int start_column, int end_line, int end_column)
      : Token(TokenKind::LPAREN, start_line, start_column, end_line,
              end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }

  std::string pp() const { return ")"; }
};

struct Whitespace : public Token {
  Whitespace(int start_line, int start_column, int end_line, int end_column)
      : Token(TokenKind::WHITESPACE, start_line, start_column, end_line,
              end_column) {}

  std::string to_string() const {
    return std::format("{}({}:{}-{}:{})", ::to_string(token_kind), start_line,
                       start_column, end_line, end_column);
  }

  std::string pp() const { return " "; }
};

using Delimiter_ = std::variant<LParen, RParen, Whitespace>;

struct Delimiter : public Delimiter_ {
  using Delimiter_::Delimiter_;

  std::string to_string() const {
    return std::visit([](auto &literal) { return literal.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &literal) { return literal.pp(); }, *this);
  }
};
/* ==================================================== */

using LispToken_ = std::variant<Symbol, Literal, Delimiter>;

struct LispToken : public LispToken_ {
  using LispToken_::LispToken_;

  std::string to_string() const {
    return std::visit([](auto &token) { return token.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &token) { return token.pp(); }, *this);
  }
};
