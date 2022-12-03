#include <fstream>
#include <iostream>
#include <vector>

char FindCommonItem(const std::vector<std::string>& lists) {
    for (char itemA : lists.front()) {
        bool contains = true;

        for (int i = 1; i < lists.size(); i++) {
            contains = contains && lists[i].find_first_of(itemA) != std::string::npos;
        }

        if (contains) {
            return itemA;
        }
    }

    return 0;
}

int GetItemPriority(char item) {
    if (std::isupper(item)) {
        return item - 'A' + 27;
    }

    return item - 'a' + 1;
}

void PartOne() {
    std::ifstream inputFile("input.txt");

    std::string line;
    int totalPriority = 0;

    while (std::getline(inputFile, line)) {
        int partitionIndex = line.size() / 2;

        std::string firstCompartment = line.substr(0, partitionIndex);
        std::string secondCompartment = line.substr(partitionIndex, line.size());

        char commonItem = FindCommonItem({ firstCompartment, secondCompartment });

        if (commonItem) {
            totalPriority += GetItemPriority(commonItem);
        }
    }

    std::cout << "Part One: " << totalPriority << std::endl;
}

void PartTwo() {
    std::ifstream inputFile("input.txt");

    std::vector<std::string> rucksacks;
    int totalPriority = 0;

    while (std::getline(inputFile, rucksacks.emplace_back())) {
        if (rucksacks.size() == 3) {
            char commonItem = FindCommonItem(rucksacks);

            totalPriority += GetItemPriority(commonItem);

            rucksacks.clear();
        }
    }

    std::cout << "Part Two: " << totalPriority << std::endl;
}

int main() {
    PartOne();
    PartTwo();
}