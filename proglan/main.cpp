#include "ast/statement.hpp"
#include "parser/lexer.hpp"
#include "parser/parser.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

std::string readFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file");
  }
  std::ostringstream ss;
  ss << file.rdbuf(); // Чтение всего содержимого в строковый поток
  return ss.str();    // Возвращаем строку
}

int main(int argc, const char* argv[]) {
  std::string input;
  try {
    if (argc < 2) {
      throw runtime_error("No input files");
    } else {
      string nameFile = argv[1];
      if (nameFile.substr(nameFile.size() - 5, 5) != ".sela") {
        throw runtime_error("File format not recognized");
      } else {
        input = readFile(nameFile);
        lexer L(input);
        vector<token> tokens = L.tokenize();
        parser P(tokens);
        shared_ptr<Statement> program = P.parse();
        //cout << program->toString();
        program->execute();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}