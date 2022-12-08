#include <iostream>
#include <fstream>
#include <vector>

typedef std::vector<std::vector<uint8_t>> Grid;

bool AreRowTreesShorter(const Grid& grid, uint8_t height, int row, int fromColumn, int toColumn)
{
    for (int column = fromColumn; column <= toColumn; column++)
    {
        uint8_t tree = grid[row][column];

        if (tree >= height)
        {
            return false;
        }
    }

    return true;
}

bool AreColumnTreesShorter(const Grid& grid, uint8_t height, int column, int fromRow, int toRow)
{
    for (int row = fromRow; row <= toRow; row++)
    {
        uint8_t tree = grid[row][column];

        if (tree >= height)
        {
            return false;
        }
    }

    return true;
}

bool IsTreeVisible(const Grid& grid, int row, int column)
{
    uint8_t height = grid[row][column];

    return AreRowTreesShorter(grid, height, row, 0, column - 1)
        || AreRowTreesShorter(grid, height, row, column + 1, grid[row].size() - 1)
        || AreColumnTreesShorter(grid, height, column, 0, row - 1)
        || AreColumnTreesShorter(grid, height, column, row + 1, grid.size() - 1);
}

int CalculateScenicScore(const Grid& grid, int row, int column)
{
    uint8_t height = grid[row][column];

    int leftViewingDistance = 0;
    int rightViewingDistance = 0;
    int upViewingDistance = 0;
    int downViewingDistance = 0;

    for (int otherColumn = column - 1; otherColumn >= 0; otherColumn--)
    {
        leftViewingDistance++;

        if (grid[row][otherColumn] >= height)
        {
            break;
        }
    }

    for (int otherColumn = column + 1; otherColumn < grid[row].size(); otherColumn++)
    {
        rightViewingDistance++;

        if (grid[row][otherColumn] >= height)
        {
            break;
        }
    }

    for (int otherRow = row - 1; otherRow >= 0; otherRow--)
    {
        upViewingDistance++;

        if (grid[otherRow][column] >= height)
        {
            break;
        }
    }

    for (int otherRow = row + 1; otherRow < grid.size(); otherRow++)
    {
        downViewingDistance++;

        if (grid[otherRow][column] >= height)
        {
            break;
        }
    }

    return leftViewingDistance
        * rightViewingDistance
        * upViewingDistance
        * downViewingDistance;
}

void PartOne(const Grid& grid)
{
    size_t count = 0;

    for (int row = 0; row < grid.size(); row++)
    {
        for (int column = 0; column < grid[column].size(); column++)
        {
            if (IsTreeVisible(grid, row, column)) {
                count++;
            }
        }
    }

    std::cout << "Part One: " << count << std::endl;
}

void PartTwo(const Grid& grid)
{
    int highestScore = 0;

    for (int row = 0; row < grid.size(); row++)
    {
        for (int column = 0; column < grid[column].size(); column++)
        {
            int score = CalculateScenicScore(grid, row, column);

            highestScore = std::max(highestScore, score);
        }
    }

    std::cout << "Part Two: " << highestScore << std::endl;
}

int main()
{
    std::ifstream inputFile("input.txt");
    std::string line;

    Grid grid;

    while (std::getline(inputFile, line))
    {
        std::vector<uint8_t>& row = grid.emplace_back();

        for (char height : line)
        {
            row.emplace_back(height - '0');
        }
    }

    PartOne(grid);
    PartTwo(grid);
}