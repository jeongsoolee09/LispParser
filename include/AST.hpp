class Quote {};

class Atom {};

class Eq {};

class Car {};

class Cdr {};

class Cons {};

class Cond {};

class Symbol {}; // a b c

class List {}; // (a b c) (a) (a (b c (d e)))

// TODO 1: Use std::variant and avoid virtual dispatch
// TODO 2: Node should be a struct with its own `LispToken` embedded
