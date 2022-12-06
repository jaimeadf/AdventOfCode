#include <iostream>
#include <fstream>
#include <string_view>

bool HasRepeatedCharacters(const std::string& packet, size_t position, size_t size) {
    std::string_view sequence(packet.c_str() + position, size);

    for (int i = 0; i < size; i++) {
        char characterA = sequence[i];
        
        for (int j = 0; j < size; j++) {
            char characterB = sequence[j];

            if (i == j) {
                continue;
            }

            if (characterA == characterB) {
                return true;
            }
        }
    }

    return false;
}

size_t FindMarker(const std::string& packet, size_t markerSize) {
    for (size_t afterIndex = markerSize; afterIndex < packet.size(); afterIndex++) {
        if (!HasRepeatedCharacters(packet, afterIndex - markerSize, markerSize)) {
            return afterIndex;
        }
    }

    return -1;
}

int main() {
    std::ifstream inputFile("input.txt");
    std::string packet;

    std::getline(inputFile, packet);

    std::cout << "Part One: " << FindMarker(packet, 4) << std::endl;
    std::cout << "Part Two: " << FindMarker(packet, 14) << std::endl;
}