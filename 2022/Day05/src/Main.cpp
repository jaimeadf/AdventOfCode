#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <regex>

struct Procedure {
    int numberOfCrates;
    char from;
    char to;
};

std::map<char, std::stack<char>> ParseShip(const std::vector<std::string>& ship) {
    std::map<char, std::stack<char>> stacks;

    const std::string& scheme = ship.back();

    for (int index = 0; index < scheme.size(); index++) {
        char key = scheme[index];

        if (key == ' ') {
            continue;
        }

        for (int level = ship.size() - 2; level >= 0; level--) {
            char crate = ship[level][index];

            if (crate != ' ') {
                stacks[key].push(ship[level][index]);
            }
        }
    }

    return stacks;
}

Procedure ParseProcedure(const std::string& command) {
    Procedure procedure;

    std::regex regex("(\\d+).+(\\d+).+(\\d+)");
    std::smatch matches;

    std::regex_search(command, matches, regex);

    procedure.numberOfCrates = std::atoi(matches[1].str().c_str());
    procedure.from = matches[2].str().front();
    procedure.to = matches[3].str().front();

    return procedure;
}

void PrintStacks(const std::map<char, std::stack<char>>& stacks) {
    for (auto pair : stacks) {
        std::cout << pair.second.top();
    }

    std::cout << std::endl;
}

void PartOne(std::map<char, std::stack<char>> stacks, const std::vector<Procedure>& procedures) {
    for (Procedure procedure : procedures) {
        auto& fromStack = stacks[procedure.from];
        auto& toStack = stacks[procedure.to];

        for (int i = 0; i < procedure.numberOfCrates; i++) {
            toStack.push(fromStack.top());
            fromStack.pop();
        }
    }

    std::cout << "Part One: ";
    PrintStacks(stacks);
}

void PartTwo(std::map<char, std::stack<char>> stacks, const std::vector<Procedure>& procedures) {
    for (Procedure procedure : procedures) {
        auto& fromStack = stacks[procedure.from];
        auto& toStack = stacks[procedure.to];

        std::vector<char> crates;

        for (int i = 0; i < procedure.numberOfCrates; i++) {
            crates.emplace_back(fromStack.top());
            fromStack.pop();
        }

        for (int i = crates.size() - 1; i >= 0; i--) {
            toStack.push(crates[i]);
        }
    }

    std::cout << "Part Two: ";
    PrintStacks(stacks);
}

int main() {
    std::ifstream inputFile("input.txt");

    std::string line;
    std::vector<std::string> ship;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            break;
        }

        ship.push_back(line);
    }

    std::map<char, std::stack<char>> stacks = ParseShip(ship);
    std::vector<Procedure> procedures;

    while (std::getline(inputFile, line)) {
        procedures.emplace_back(ParseProcedure(line));
    }

    PartOne(stacks, procedures);
    PartTwo(stacks, procedures);
}