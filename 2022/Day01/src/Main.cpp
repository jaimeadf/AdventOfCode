#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::ifstream inputFile("input.txt");
    std::string line;

    std::vector<std::vector<int>> elves;

    elves.emplace_back();

    while (std::getline(inputFile, line)) {
        if (line == "") {
            elves.emplace_back();
            continue;
        }

        elves.back().push_back(std::atoi(line.c_str()));
    }

    std::vector<int> totals;

    for (auto elf : elves) {
        totals.emplace_back(std::accumulate(elf.begin(), elf.end(), 0));
    }

    std::sort(totals.begin(), totals.end(), std::greater<int>());

    std::cout << "First: " << totals[0] << std::endl;
    std::cout << "Second: " << totals[1] << std::endl;
    std::cout << "Third: " << totals[2] << std::endl;

    std::cout << std::endl;
    std::cout << "Sum: " << totals[0] + totals[1] + totals[2] << std::endl;
}