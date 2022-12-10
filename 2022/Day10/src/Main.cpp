#include <iostream>
#include <fstream>

int main()
{
    std::ifstream inputFile("input.txt");
    std::string line;

    size_t cycles = 0;
    
    int x = 1;
    int temp = 0;

    int signalSum = 0;

    while (true)
    {
        cycles++;

        size_t column = (cycles - 1) % 40;

        std::cout << (column >= x - 1 && column <= x + 1 ? '#' : '.');

        if (column == 40 - 1)
        {
            std::cout << std::endl;
        }

        if (cycles >= 20 && (cycles - 20) % 40 == 0)
        {
            signalSum += cycles * x;
        }

        if (temp != 0)
        {
            x += temp;
            temp = 0;

            continue;
        }
                
        if (!std::getline(inputFile, line))
        {
            break;
        }
        
        size_t separatorIndex = line.find(' ');
        std::string instruction = line.substr(0, separatorIndex);
        std::string argument = line.substr(separatorIndex + 1);

        if (instruction == "addx")
        {
            temp = std::atoi(argument.c_str());
        }
    }

    std::cout << std::endl;
    std::cout << "Part One: " << signalSum << std::endl;
}