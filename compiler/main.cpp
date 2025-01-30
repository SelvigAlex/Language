#include "lexer.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "statement.hpp"
#include "parser.hpp"
#include <cmath>
#include <sstream>


using namespace std;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    std::ostringstream ss;
    ss << file.rdbuf(); // Чтение всего содержимого в строковый поток
    return ss.str(); // Возвращаем строку
}


int main(int argc, char *argv[]) {
    std::string input;
    try {
        input = readFile("program.txt");
        //std::cout << "File content: " << input << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    lexer L(input);
    vector<token> tokens = L.tokenize();

    // // Вывод токенов
    // for (const auto &elem : tokens) {
    //     cout << elem.toString() << ' ' << elem.getLexeme() << '\n';
    // }

    // Разбор и выполнение инструкций
    parser P(tokens);
    vector<shared_ptr<Statement>> statements = P.parse();

    // //Вывод представлений инструкций
    // for (const auto &state : statements) {
    //     cout << state->toString();
    // }
    // cout << '\n';

    // Выполнение инструкций
    for (const auto &state : statements) {
        state->execute();
    }
    cout << '\n';
    // Закрытие файла
    return 0;
}