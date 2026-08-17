#ifndef LITTLELISPPARSER_TOKEN
#define LITTLELISPPARSER_TOKEN

#include <cctype>
#include <format>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

// State transition table on a constant memory
// State transition should encode everything

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
  Eof,
  WHITESPACE
};

std::string to_string(TokenKind token);

/**
 * Constructs a `TokenKind` out of a given string. It is the inverse of
 * `to_string`.
 */
std::optional<TokenKind> from_string(std::string_view keyword_name);

struct Range {
  // Range(int start_line, int start_column, int end_line, int end_column)
  //     : start_line(start_line), start_column(start_column),
  //     end_line(end_line),
  //       end_column(end_column) {}

  std::size_t start_line = 0;
  std::size_t start_column = 0;
  std::size_t end_line = 0;
  std::size_t end_column = 0;

  bool operator==(const Range &) const = default;

  std::string to_string() const {
    return std::format("{}", start_line, start_column, end_line, end_column);
  }
};

/**
  Interface for a token.
  It is up to each TokenSig class to implement its
  own `to_string`, `pp`, `get_kind` functions.
*/

/* Const-correctness is up to the owner / borrower */

struct TokenSig {
  TokenSig(Range range) : range(range) {}

  /** Read-only view into the range field */
  const Range &get_range() const;

  /** Read & Write view into the range field */
  Range &get_range();

  bool operator==(const TokenSig &) const = default;

protected:
  Range range;
};

/* ==================== 1. Symbols ==================== */

struct QuoteKeywordSymbol : public TokenSig {
  QuoteKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::QUOTE; }

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "quote"; }

  bool operator==(const QuoteKeywordSymbol &) const = default;
};

struct AtomKeywordSymbol : public TokenSig {
  AtomKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::ATOM; }

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "atom"; }

  bool operator==(const AtomKeywordSymbol &) const = default;
};

struct EqKeywordSymbol : public TokenSig {
  EqKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::EQ; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }
  std::string pp() const { return "eq"; }

  bool operator==(const EqKeywordSymbol &) const = default;
};

struct CarKeywordSymbol : public TokenSig {
  CarKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::CAR; }

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }
  std::string pp() const { return "car"; }

  bool operator==(const CarKeywordSymbol &) const = default;
};

struct CdrKeywordSymbol : public TokenSig {
  CdrKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::CDR; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }
  std::string pp() const { return "cdr"; }

  bool operator==(const CdrKeywordSymbol &) const = default;
};

struct ConsKeywordSymbol : public TokenSig {
  ConsKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::CONS; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }
  std::string pp() const { return "cons"; }

  bool operator==(const ConsKeywordSymbol &) const = default;
};

struct CondKeywordSymbol : public TokenSig {
  CondKeywordSymbol(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::COND; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }
  std::string pp() const { return "cond"; }

  bool operator==(const CondKeywordSymbol &) const = default;
};

struct UserSymbol : public TokenSig {
  UserSymbol(std::string_view name, Range range)
      : TokenSig(range), name(name) {}

  TokenKind get_kind() const { return TokenKind::UserSymbol; }
  std::string to_string() const {
    return std::format(
        "{}(\"{}\"{})", ::to_string(get_kind()), name, range.to_string());
  }
  std::string pp() const { return name; }

  bool operator==(const UserSymbol &) const = default;

  std::string name;
};

struct Symbol;

using KeywordSymbol_ = std::variant<QuoteKeywordSymbol,
                                    AtomKeywordSymbol,
                                    EqKeywordSymbol,
                                    CarKeywordSymbol,
                                    CdrKeywordSymbol,
                                    ConsKeywordSymbol,
                                    CondKeywordSymbol>;

struct KeywordSymbol : public KeywordSymbol_ {
  using KeywordSymbol_::KeywordSymbol_;

  TokenKind get_kind() const {
    return std::visit(
        [](auto &keyword_symbol) { return keyword_symbol.get_kind(); }, *this);
  }

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
  using Symbol_::Symbol_;

  std::string to_string() const {
    return std::visit(
        [](auto &keyword_symbol) { return keyword_symbol.to_string(); }, *this);
  }

  std::string pp() const {
    return std::visit([](auto &keyword_symbol) { return keyword_symbol.pp(); },
                      *this);
  }

  TokenKind get_kind() const {
    return std::visit([](auto &symbol) { return symbol.get_kind(); }, *this);
  }

  static Symbol from_string(std::string_view keyword_name, Range range);
};

inline Symbol Symbol::from_string(std::string_view keyword_name, Range range) {
  std::optional<TokenKind> token_kind = ::from_string(keyword_name);
  if (token_kind.has_value()) {
    switch (token_kind.value()) {
    case TokenKind::QUOTE:
      return QuoteKeywordSymbol{ range }; // Unambiguously a constructor call,
    case TokenKind::ATOM:
      return AtomKeywordSymbol{ range };
    case TokenKind::EQ:
      return EqKeywordSymbol{ range };
    case TokenKind::CAR:
      return CarKeywordSymbol{ range };
    case TokenKind::CDR:
      return CdrKeywordSymbol{ range };
    case TokenKind::CONS:
      return ConsKeywordSymbol{ range };
    case TokenKind::COND:
      return CondKeywordSymbol{ range };
    default:
      throw std::invalid_argument(std::string{ "Got unexpected type" } +
                                  std::string{ keyword_name });
    }
  } else {
    return UserSymbol{ keyword_name, range };
  }
}

/* ==================================================== */

/* =================== 2. Literals ==================== */
/** A `t` literal. */
struct TLiteral : public TokenSig {
  TLiteral(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::TLiteral; }
  bool operator==(const TLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "t"; }
};

/** A `nil` literal. */
struct NilLiteral : public TokenSig {
  NilLiteral(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::NilLiteral; }
  bool operator==(const NilLiteral &) const = default;

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "nil"; }
};

/** An integer literal. */
struct IntLiteral : public TokenSig {
  IntLiteral(int value, Range range) : TokenSig(range), value(value) {}

  const int value;

  TokenKind get_kind() const { return TokenKind::IntLiteral; }
  bool operator==(const IntLiteral &) const = default;

  std::string to_string() const {
    return std::format(
        "{}({}, {})", ::to_string(get_kind()), value, range.to_string());
  }

  std::string pp() const { return std::to_string(value); }
};

/** A string literal. */
struct StringLiteral : public TokenSig {
  StringLiteral(std::string value, Range range)
      : TokenSig(range), value(value) {}

  const std::string value;

  TokenKind get_kind() const { return TokenKind::StringLiteral; }
  bool operator==(const StringLiteral &) const = default;

  std::string to_string() const {
    return std::format(
        "{}({}, {})", ::to_string(get_kind()), value, range.to_string());
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

  TokenKind get_kind() const {
    return std::visit([](auto &literal) { return literal.get_kind(); }, *this);
  }
};
/* ==================================================== */

/* ================= 3. Delimiters ==================== */
/** A delimiter token that's purely needed for lexing. */
struct LParen : public TokenSig {
  LParen(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::LPAREN; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "("; }
};

struct RParen : public TokenSig {
  RParen(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::RPAREN; }
  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return ")"; }
};

struct Eof : public TokenSig {
  Eof(Range range) : TokenSig(range) {}

  TokenKind get_kind() const { return TokenKind::Eof; }

  std::string to_string() const {
    return std::format("{}({})", ::to_string(get_kind()), range.to_string());
  }

  std::string pp() const { return "EOF"; }
};

using Delimiter_ = std::variant<LParen, RParen>;

struct Delimiter : public Delimiter_ {
  using Delimiter_::Delimiter_;

  std::string to_string() const {
    return std::visit([](auto &delimiter) { return delimiter.to_string(); },
                      *this);
  }

  std::string pp() const {
    return std::visit([](auto &delimiter) { return delimiter.pp(); }, *this);
  }

  TokenKind get_kind() const {
    return std::visit([](auto &delimiter) { return delimiter.get_kind(); },
                      *this);
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

  TokenKind get_kind() const {
    return std::visit([](auto &token) { return token.get_kind(); }, *this);
  }

  /* TODO: Expose get_range */
  const Range &get_range() const {
    /* TODO: Complete the delegation */
    return std::visit([](auto &token) { return token.get_range(); }, *this);
  }
};

struct LiteralPrinter {
  LiteralPrinter() {}

  /*
1. TLiteral -> "t"
2. NilLiteral -> "nil"
3. IntLiteral(n) -> std::to_string(n)
4. StringLiteral(s) -> s
         */

  std::string operator()(TLiteral) { return "t"; }
  std::string operator()(NilLiteral) { return "nil"; }
  std::string operator()(IntLiteral int_literal) {
    return std::to_string(int_literal.value);
  }
  std::string operator()(StringLiteral string_literal) {
    return string_literal.value;
  }
};

// template <typename ReturnType, typename THandler, typename NilHandler,
//           typename IntHandler, typename StringHandler>
// struct LiteralVisitor {
//   LiteralVisitor(THandler t_handler, NilHandler nil_handler,
//                    IntHandler int_handler, StringHandler string_handler)
//       : t_handler(t_handler), nil_handler(nil_handler),
//         int_handler(int_handler), string_handler(string_handler) {}

//   /*
//   1. TLiteral -> "t"
//   2. NilLiteral -> "nil"
//   3. IntLiteral(n) -> std::to_string(n)
//   4. StringLiteral(s) -> s
//   */

//   THandler &t_handler;
//   NilHandler &nil_handler;
//   IntHandler &int_handler;
//   StringHandler &string_handler;

//   ReturnType operator()(TLiteral) { return t_handler(); }
//   ReturnType operator()(NilLiteral) { return nil_handler(); }
//   ReturnType operator()(IntLiteral int_literal) {
//     return int_handler(int_literal);
//   }
//   ReturnType operator()(StringLiteral string_literal) {
//     return string_handler(
//         string_literal); // string_handler == [](&auto string_literal)
//         {return
//                          // Value::String} ==> string_handler() ==
//                          Value::String
//   }
// };

/*
std::visit(LiteralPrinter{}, *literal)

Value value = std::visit(LiteralVisitor<Value>{[](&auto t_literal){ return
Value::T },
                                   [](&auto nil_literal){ return Value::Nil},
                                                                                                                                         [](&auto int_literal){return Value::Int},}, *literal)

std::visit(Value::String, *literal)
*/

// template<typename T>
// ReturnType operator()(T t) {
//   return not_a_literal_handler();
// }

template <typename ReturnType,
          typename THandler,
          typename NilHandler,
          typename IntHandler,
          typename StringHandler,
          typename CatchallHandler>
struct LiteralVisitor {
  LiteralVisitor(THandler t_handler,
                 NilHandler nil_handler,
                 IntHandler int_handler,
                 StringHandler string_handler,
                 CatchallHandler catch_all_handler)
      : t_handler(t_handler), nil_handler(nil_handler),
        int_handler(int_handler), string_handler(string_handler),
        catch_all_handler(catch_all_handler) {}

  static_assert(std::is_invocable_v<THandler, TLiteral>,
                "THandler should be invocable with a TLiteral");

  /*
    1. TLiteral -> "t"
    2. NilLiteral -> "nil"
    3. IntLiteral(n) -> std::to_string(n)
    4. StringLiteral(s) -> s
    */

  THandler &t_handler;
  NilHandler &nil_handler;
  IntHandler &int_handler;
  StringHandler &string_handler;
  CatchallHandler &catch_all_handler;

  /* ReturnType to be a enum class or a std::variant. */
  ReturnType operator()(TLiteral) { return t_handler(); }
  ReturnType operator()(NilLiteral) { return nil_handler(); }
  ReturnType operator()(IntLiteral int_literal) {
    return int_handler(int_literal);
  }
  ReturnType operator()(StringLiteral string_literal) {
    return string_handler(
        string_literal); // string_handler == [](&auto string_literal) {return
                         // Value::String} ==> string_handler() == Value::String
  }
  template <typename T>
  ReturnType operator()(T t) { // Don't care which actual type T is
    return catch_all_handler(t);
  }
};

#endif
