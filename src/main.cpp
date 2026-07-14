#include <iostream>
#include "lexer.hpp"

int main() {
	const char* case1 = "()";
	const char* case2 = "(quote car)";
	const char* case3 = "(cond car cdr)";
	const char* case4 = "(quote eq)";
	const char* case5 = "(car a b)";
	const char* case6 = "";
	const char* case7 = "!";

	std::vector<LispToken> case1_parsed = lex(case1);
	std::vector<LispToken> case2_parsed = lex(case2);
	std::vector<LispToken> case3_parsed = lex(case3);
	std::vector<LispToken> case4_parsed = lex(case4);
	std::vector<LispToken> case5_parsed = lex(case5);
	std::vector<LispToken> case6_parsed = lex(case6);
	std::vector<LispToken> case7_parsed = lex(case7);

	for (LispToken token : case1_parsed) {
		std::cout << to_string(token) << "\n";
  }
	
	std::cout << "==============================" << "\n";

	for (LispToken token : case2_parsed) {
		std::cout << to_string(token) << "\n";
  }

	std::cout << "==============================" << "\n";

	for (LispToken token : case3_parsed) {
		std::cout << to_string(token) << "\n";
  }

	std::cout << "==============================" << "\n";

	for (LispToken token : case4_parsed) {
		std::cout << to_string(token) << "\n";
  }

	std::cout << "==============================" << "\n";

	for (LispToken token : case5_parsed) {
		std::cout << to_string(token) << "\n";
  }

	std::cout << "==============================" << "\n";

	for (LispToken token : case6_parsed) {
		std::cout << to_string(token) << "\n";
  }

  std::cout << "==============================" << "\n";

	for (LispToken token : case7_parsed) {
		std::cout << to_string(token) << "\n";
  }

  return 0;
}
