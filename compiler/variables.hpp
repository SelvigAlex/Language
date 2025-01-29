#include <iostream>
#include <unordered_map>
#include <string>

class Variables {
public:
    static bool isExists(const std::string& key) {
        return variables.find(key) != variables.end();
    }

    static double get(const std::string& key) {
        if (!isExists(key)) return 0.0;
        return variables.at(key);
    }

    static void set(const std::string& key, double value) {
        variables.insert({key, std::move(value)});
    }
private:
    static std::unordered_map<std::string, double> variables;
};


std::unordered_map<std::string, double> Variables::variables {
        {"PI", 3.141592653589793},
        {"E", 2.718281828459045}
};