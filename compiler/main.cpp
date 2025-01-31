#include "ast/statement.hpp"
#include "parser/lexer.hpp"
#include "parser/parser.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

std::string readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file");
  }

  std::ostringstream ss;
  ss << file.rdbuf(); // Чтение всего содержимого в строковый поток
  return ss.str();    // Возвращаем строку
}

int main() {
  std::string input;
  try {
    input = readFile("program.txt");
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  lexer L(input);
  vector<token> tokens = L.tokenize();
  parser P(tokens);
//   for (const auto &t : tokens) {
//     cout << t.toString() << '\n';
//   }
  shared_ptr<Statement> program = P.parse();
  //cout << program->toString();
  program->execute();
  cout << '\n';
  return 0;
}

/*number = 10

*/
