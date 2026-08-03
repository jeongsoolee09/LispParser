#ifndef LITTLELISPPARSER_LEXER
#define LITTLELISPPARSER_LEXER

#include "token.hpp"
#include <vector>

std::vector<LispToken> lex(std::string_view source);

#endif
