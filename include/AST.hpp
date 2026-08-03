#ifndef LITTLELISPPARSER_AST
#define LITTLELISPPARSER_AST

#include "token.hpp"
#include <variant>
#include <vector>

struct SymbolNode {
  const Symbol* token;
};

struct LiteralNode {
  const Literal* token;
};

using Atom = std::variant<SymbolNode, LiteralNode>;

// struct List {}; // (a b c) (a) (a (b c (d e)))

struct Node;

using List = std::vector<Node>;

using Node_ = std::variant<Atom, List>;

using Program = std::vector<Node>;

struct Node : public Node_ {
	using Node_::Node_;
};

// TODO 1: Use std::variant and avoid virtual dispatch
// TODO 2: Node should be a struct with its own `LispToken` embedded

// type 'a List = Nil | Cons of ('a * List)

#endif
