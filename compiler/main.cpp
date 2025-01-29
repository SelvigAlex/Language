// #include "lexer.hpp"
// #include <iostream>
// #include <string>
// #include <vector>
// #include "statement.hpp"
// #include "parser.hpp"


// using namespace std;

// int main(int argc, char *argv[]) {
//   string input = "print";
//   lexer L(input);
//   vector<token> tokens = L.tokenize();
//   for (auto &elem : tokens) {
//     cout << elem.toString() << ' ' << elem.getLexeme() << '\n';
//   }
//   parser P(tokens);
//   vector<unique_ptr<Statement>> statements = P.parse();
//   for (auto &state : statements) {
//     cout << state.get()->toString();
//   }
//   for (auto &state : statements) {
//     state.get()->execute();
//   }
//   cout << '\n';
//   cout << Variables::get("word") << '\n';
//   cout << Variables::get("word2") << '\n';
//   return 0;
// }


#include "lexer.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "statement.hpp"
#include "parser.hpp"


using namespace std;

int main(int argc, char *argv[]) {
  std::ifstream in("program.txt"); // окрываем файл для чтения
  if (in.is_open()) {
    string input;
    while (std::getline(in, input)) {
      input = "word = 2 * 2 - 1\nprint word";
      lexer L(input);
      vector<token> tokens = L.tokenize();
      for (auto &elem : tokens) {
        cout << elem.toString() << ' ' << elem.getLexeme() << '\n';
      }
      parser P(tokens);
      vector<unique_ptr<Statement>> statements = P.parse();
      for (auto &state : statements) {
        cout << state.get()->toString();
      }
      for (auto &state : statements) {
        state.get()->execute();
      }
      cout << '\n';
      cout << Variables::get("word") << '\n';
      cout << Variables::get("word2") << '\n';
      input.clear();
    }
  }



  
  
  
  
  
  in.close(); 
  return 0;
}