#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <numeric>

#include "Monkey.cpp"

int ExtractLastNumber(const std::string& text)
{
    return std::atoi(text.c_str() + text.find_last_not_of("0123456789") + 1);
}

std::vector<Monkey> ReadMonkeys(const std::string& fileName)
{
    std::ifstream inputFile(fileName);
    std::string line;

    std::vector<Monkey> monkeys;
    Monkey* currentMonkey;

    while (std::getline(inputFile, line))
    {
        size_t colonIndex = line.find(':');
        std::string attribute = line.substr(0, colonIndex);
        std::string value = line.substr(colonIndex + 1);

        if (attribute.find("Monkey") != std::string::npos)
        {
            currentMonkey = &monkeys.emplace_back();
            continue;
        }

        if (attribute.find("Starting items") != std::string::npos)
        {
            std::stringstream ss(value);

            for (int item; ss >> item;) {
                currentMonkey->Items.push_back(item);

                if (ss.peek() == ',' || ss.peek() == ' ')
                {
                    ss.ignore();
                }
            }
            
            continue;
        }

        if (attribute.find("Operation") != std::string::npos)
        {
            currentMonkey->ItemOperation = Operation::Parse(value);
            continue;
        }

        if (attribute.find("Test") != std::string::npos)
        {
            currentMonkey->ItemTest.DivideBy = ExtractLastNumber(value);
            continue;
        }

        if (attribute.find("If true") != std::string::npos)
        {
            currentMonkey->ItemTest.IfTrueMonkey = ExtractLastNumber(value);
            continue;
        }

        if (attribute.find("If false") != std::string::npos)
        {
            currentMonkey->ItemTest.IfFalseMonkey = ExtractLastNumber(value);
            continue;
        }
    }

    return monkeys;
}

size_t CalculateMonkeyBussinessLevel(
    std::vector<Monkey> monkeys,
    size_t rounds,
    std::function<unsigned long long(unsigned long long)> manager
)
{
    std::vector<size_t> inspections(monkeys.size());

    for (int round = 1; round <= rounds; round++)
    {
        for (size_t i  = 0; i < monkeys.size(); i++)
        {
            Monkey& monkey = monkeys[i];

            while (!monkey.Items.empty())
            {
                inspections[i]++;
                
                unsigned long long initialWorryLevel = monkey.Items.front();
                unsigned long long currentWorryLevel = manager(monkey.ItemOperation.Evaluate(initialWorryLevel));

                size_t toMonkeyIndex = monkey.ItemTest.Check(currentWorryLevel);

                monkeys[toMonkeyIndex].Items.push_front(currentWorryLevel);

                monkey.Items.pop_front();
            }
        }
    }

    std::sort(inspections.begin(), inspections.end(), std::greater<size_t>());

    return inspections[0] * inspections[1];
}

void PartOne(const std::vector<Monkey>& monkeys)
{
    size_t bussinessLevel = CalculateMonkeyBussinessLevel(monkeys, 20, [](unsigned long long worryLevel) {
        return worryLevel / 3;
    });

    std::cout << "Part One: " << bussinessLevel << std::endl;
}

void PartTwo(const std::vector<Monkey>& monkeys)
{
    size_t lcm = 1;

    for (const Monkey& monkey : monkeys)
    {
        lcm *= monkey.ItemTest.DivideBy;
    }

    size_t bussinessLevel = CalculateMonkeyBussinessLevel(monkeys, 10000, [lcm](unsigned long long worryLevel) {
        return worryLevel % lcm;
    });

    std::cout << "Part Two: " << bussinessLevel << std::endl;
}

int main()
{
    std::vector<Monkey> monkeys = ReadMonkeys("input.txt");

    PartOne(monkeys);
    PartTwo(monkeys);
}