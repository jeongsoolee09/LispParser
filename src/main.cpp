#include <iostream>
#include <glog/logging.h>
#include "lexer.hpp"

int main(int argc, char* argv[]) {
  FLAGS_logtostderr = true;
	google::InitGoogleLogging(argv[0]);

	// Drop into the REPL maybe
	const char* case1 = "(cons a (b c))";
	lex(case1);
	
  return 0;
}
