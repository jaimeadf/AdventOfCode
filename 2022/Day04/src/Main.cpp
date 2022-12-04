#include <fstream>
#include <iostream>
#include <array>
#include <tuple>

std::array<int, 2> ParseSectionRange(const std::string& text) {
    size_t delimiterIndex = text.find('-');

    std::string minimumText = text.substr(0, delimiterIndex);
    std::string maximumText = text.substr(delimiterIndex + 1);

    return { std::atoi(minimumText.c_str()), std::atoi(maximumText.c_str()) };
}

std::tuple<std::array<int, 2>, std::array<int, 2>> ParsePair(const std::string& text) {
    size_t delimiterIndex = text.find_first_of(',');
        
    std::string elfA = text.substr(0, delimiterIndex);
    std::string elfB = text.substr(delimiterIndex + 1);

    return std::make_tuple(ParseSectionRange(elfA), ParseSectionRange(elfB));
}

int main() {
    std::ifstream inputFile("input.txt");

    std::string line;

    size_t overlapPartOneCount = 0;
    size_t overlapPartTwoCount = 0;

    while (std::getline(inputFile, line)) {
        auto pair = ParsePair(line);

        auto rangeA = std::get<0>(pair);
        auto rangeB = std::get<1>(pair);

        if (
            rangeA[0] <= rangeB[0] && rangeA[1] >= rangeB[1] ||
            rangeB[0] <= rangeA[0] && rangeB[1] >= rangeA[1]
        ) {
            overlapPartOneCount++;
        }

        if (
            rangeA[0] <= rangeB[0] && rangeA[1] >= rangeB[0] || rangeA[0] <= rangeB[1] && rangeA[1] >= rangeB[1] ||
            rangeB[0] <= rangeA[0] && rangeB[1] >= rangeA[0] || rangeB[0] <= rangeA[1] && rangeB[1] >= rangeA[1]
        ) {
            overlapPartTwoCount++;
        }
    }

    std::cout << "Part One: " << overlapPartOneCount << std::endl;
    std::cout << "Part Two: " << overlapPartTwoCount << std::endl;
}