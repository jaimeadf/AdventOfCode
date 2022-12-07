#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct Command
{
    std::string Action;
    std::string Arguments;
};

bool IsCommand(const std::string& line)
{
    return line.front() == '$';
}

Command ParseCommand(const std::string& line)
{
    Command command;

    size_t actionIndex = line.find(' ');
    size_t argumentsIndex = line.find(' ', actionIndex + 1);

    command.Action = line.substr(actionIndex + 1, argumentsIndex - actionIndex - 1);
    command.Arguments = line.substr(argumentsIndex + 1);

    return command;
}

void PartOne(const std::map<std::string, size_t>& directories)
{
    size_t sum = 0;

    for (auto [path, size] : directories)
    {
        if (size < 100000)
        {
            sum += size;
        }
    }

    std::cout << "Part One: " << sum << std::endl;
}

void PartTwo(const std::map<std::string, size_t>& directories)
{
    size_t targetSpace = 30000000 - (70000000 - directories.at("/"));
    size_t smallestTargetSize = SIZE_MAX;

    for (auto [path, size] : directories)
    {
        if (size >= targetSpace)
        {
            smallestTargetSize = std::min(smallestTargetSize, size);
        }
    }

    std::cout << "Part Two: " << smallestTargetSize << std::endl;
}

int main()
{
    std::ifstream inputFile("input.txt");
    std::string line;

    std::map<std::string, size_t> directories;
    std::string path;

    while (std::getline(inputFile, line))
    {
        if (IsCommand(line))
        {
            Command command = ParseCommand(line);

            if (command.Action == "cd")
            {
                if (command.Arguments.front() == '/')
                {
                    path = command.Arguments;
                }
                else if (command.Arguments == "..")
                {
                    path = path.substr(0, path.rfind('/', path.size() - 2) + 1);
                }
                else
                {
                    path += command.Arguments + '/';
                }
            }

            continue;
        }

        size_t delimiterIndex = line.find(' ');

        std::string partOne = line.substr(0, delimiterIndex);
        std::string partTwo = line.substr(delimiterIndex + 1);

        if (partOne == "dir")
        {
            directories[path + partTwo + '/'] = 0;
            continue;
        }
        
        size_t currentPathSeparatorIndex = 0;
        size_t fileSize = std::atoi(partOne.c_str());

        directories["/"] += fileSize;

        while (currentPathSeparatorIndex + 1 < path.size())
        {
            size_t nextPathSeparatorIndex = path.find('/', currentPathSeparatorIndex + 1);
            std::string subPath = path.substr(0, nextPathSeparatorIndex + 1);

            directories[subPath] += fileSize;

            currentPathSeparatorIndex = nextPathSeparatorIndex;
        }
    }

    PartOne(directories);
    PartTwo(directories);
}