#include "lexer.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "statement.hpp"
#include "parser.hpp"

#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream in("program.txt"); // открываем файл для чтения
    if (!in) { 
        cerr << "Ошибка открытия файла!" << endl;
        return 1; // Возвращаем код ошибки, если файл не открылся
    }

    string input;
    while (getline(in, input)) {
        // Лексический анализ
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
        
    }
    cout << '\n';
    // Закрытие файла
    in.close();
    return 0;
}
