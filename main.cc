#include <iostream>
#include <iostream>
#include <vector>
#include "scanner.h"
#include "parser.h"

int main(int argc, char const *argv[]) {
  std::string line, code;
  while (std::getline(std::cin, line)) {
    code += line;
  }
  try {
    std::vector<ScannerToken> tokens = scan(code);
    tokens.insert(tokens.begin(), ScannerToken{BOF, "BOF"});
    tokens.emplace_back(ScannerToken{END, "END"});
    std::cout << "Tokens: " << std::endl;
    for (const auto &token : tokens) {
        std::cout << token;
    }
    std::cout << std::endl << std::endl;;

    const std::vector<std::vector<ParserToken> > result = parse(tokens);
    std::cout << "Productions: " << std::endl;
    for (const auto &p : result) {
      bool first = true;
      for (const auto &i : p) {
        std::cout << i << (first ? " -> " : " ");
        first = false;
      }
      std::cout << std::endl;
    }
  }
  catch (const ScanningException &e) {
    std::cerr << e.getMessage() << std::endl;
  }
  catch (const ParsingException &e) {
    std::cerr << e.getMessage() << std::endl;
  }

  return 0;
}
